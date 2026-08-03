#include <Storage/NamedSpace.hpp>

// ----------------------------------------------------------------------------------------------------

bool NamedSpace::FindRecordByName(const StaticText32& name, ScanRecord& record) const
{
	for (const ScanRecord& rec : records)
	{
		// Такая запись есть, новую не добавляем
		if (rec.name == name)
		{
			record = rec;
			return true;
		}
	}
	return false;
}

bool NamedSpace::Allocate(uint16_t& addr_lnk, const StaticText32& name, const NamedDataHeader& data)
{
	addr_lnk = 0;

	if (records.Count() == 0)
	{
		ScanRecord data_record(addr_lnk, name, data.size);
		records.Push(data_record);
		return true;
	}

	int block_size = (int)(sizeof(NamedDataHeader) + data.size);

	// Итератор на начало списка
	auto it1 = records.begin();
	auto it2 = records.begin();
	if (it1 != records.end())
		it2++;

	// Такая запись есть, новую не добавляем
	if (Exists(name))
		return false;

	// Если перед первой записью есть место, вставляем туда
	if ((int)it1->address >= block_size)
	{
		ScanRecord data_record(addr_lnk, name, data.size);
		records.Push(data_record);
		return true;
	}

	// Пока второй итератор не уперся в конец памяти
	while (it2 != records.end())
	{
		addr_lnk = it1->NextAddress();

		// Если запись можно уместить между двумя другими, вставляем ДО итератора it2
		if (addr_lnk + block_size < (int)it2->address)
		{
			ScanRecord data_record(addr_lnk, name, data.size);
			records.Insert(it2, data_record);
			return true;
		}

		it1++;
		it2++;
	}

	// Если запись можно уместить после последней записи
	ScanRecord& last_record = records.Back();
	addr_lnk = last_record.NextAddress();

	// Не добавляем, если нет места
	if (addr_lnk + block_size >= space_size)
		return false;

	ScanRecord data_record(addr_lnk, name, data.size);
	records.Push(data_record);
	return true;
}

bool NamedSpace::Free(const StaticText32& name)
{
	ScanRecord rec;
	if (FindRecordByName(name, rec))
	{
		const uint16_t EMPTY_SIZE = 16;
		char empty[EMPTY_SIZE]{};

		for (uint16_t i = 0; i < rec.size + DATA_NAME_SIZE + 2; i += EMPTY_SIZE)
		{
			uint16_t _size = (rec.size + DATA_NAME_SIZE + 2) - i;
			if (_size > EMPTY_SIZE)
				_size = EMPTY_SIZE;
			storage->Write(
				start_address + rec.address + i,
				&empty, _size
			);
		}

		records.RemoveIf(
			[rec](ScanRecord& record)
			{
				return record.address == rec.address;
			}
		);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------------------------------

#include <LetoAPI_V1/LetoAPI_V1.h>

bool NamedSpace::Create(const StaticText32& name, const void* data, uint16_t data_size)
{
	NamedDataHeader header;
	header.size = data_size;
	leto_api_v1->Text->FormatText(header.name, sizeof(header.name), name.ConstChar());

	/// TODO: Нужно ли жестко привязывать внутреннюю логику к LetoAPI ?

	unsigned short _addr = 0;
	if (!Allocate(_addr, name, header))
		return false;

	storage->Write(start_address + _addr,						&header,	sizeof(header));
	storage->Write(start_address + _addr + (int) sizeof(header),	data,		data_size);
	return true;
}

bool NamedSpace::Read(const StaticText32& name, void* data, uint16_t data_size)
{
	ScanRecord rec;
	if (FindRecordByName(name, rec))
	{
		if (rec.size != data_size)
			return false;
		storage->Read(start_address + rec.DataAddress(), data, data_size);
		return true;
	}
	return false;
}

bool NamedSpace::Update(const StaticText32& name, const void* data, uint16_t data_size)
{
	ScanRecord rec;
	if (FindRecordByName(name, rec))
	{
		if (rec.size != data_size)
			return false;
		
		storage->Write(start_address + rec.DataAddress(), data, data_size);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------------------------------


NamedSpace::NamedSpace(uint16_t start_address, uint16_t space_size, IBinaryStorage* storage)
	: start_address{ start_address }, space_size{ space_size }, storage{ storage }
{
}

int NamedSpace::GetUsedBytes() const
{
	if (records.Count() == 0)
		return 0;
	return records.Back().NextAddress();
}

int NamedSpace::GetTotalBytes() const
{
	return space_size;
}

bool NamedSpace::Exists(const StaticText32& name) const
{
	ScanRecord record;
	return FindRecordByName(name, record);
}

void NamedSpace::Clear()
{
	constexpr uint16_t CLEAR_SIZE = 32;
	char empty[CLEAR_SIZE]{};
	for (uint16_t addr = start_address; addr < start_address + space_size; addr += CLEAR_SIZE)
	{
		uint16_t _size = (start_address + space_size) - addr;
		if (_size > CLEAR_SIZE)
			_size = CLEAR_SIZE;

		storage->Write(addr, empty, _size);
	}
}

#include <string.h>

template <typename T>
class DataByBytes
{
protected:
	char data[sizeof(T)]{};
	int cur_idx = 0;

public:
	DataByBytes() = default;

	T* Get()
	{
		return (T*)data;
	}
	
	int GetCurIdx() const { return cur_idx; }
	void Reset() { cur_idx = 0; memset(data, 0, sizeof(T)); }

	void SetByte(char byte)
	{
		data[cur_idx] = byte;
		cur_idx++;
	}

	int Size() const { return sizeof(T); }

	bool Ready() const
	{
		return cur_idx >= sizeof(T);
	}
};

void NamedSpace::Scan()
{
	bool b_start = false;
	uint16_t record_start_addr = 0;

	DataByBytes<NamedDataHeader> HEADER;

	constexpr uint16_t READ_SIZE = 32;
	const char sign_template[2] = NAMED_DATA_HEADER_SIGNATURE;

	records.Clear();

	for (uint16_t high_addr = start_address; high_addr < start_address + space_size; high_addr += READ_SIZE)
	{
		char data[32]{};
		storage->Read(high_addr, data, READ_SIZE);
		for (uint16_t low_addr = 0; low_addr < READ_SIZE; ++low_addr)
		{
			if (!b_start && (data[low_addr] == 0x00 || data[low_addr] == 0xFF))
				continue;

			// Старт записывается как относительное смещение от стартового адреса
			if (!b_start)
			{
				record_start_addr = (high_addr - start_address) + low_addr;
				b_start = true;
			}

			if (!HEADER.Ready())
			{
				HEADER.SetByte(data[low_addr]);
				if (HEADER.GetCurIdx() == 1 &&
					HEADER.Get()->SIGNATURE[0] != sign_template[0] ||
					HEADER.GetCurIdx() == 2 &&
					HEADER.Get()->SIGNATURE[1] != sign_template[1])
				{
					b_start = false;
					HEADER.Reset();
				}
			}
			else
			{
				ScanRecord record(record_start_addr, HEADER.Get()->name, HEADER.Get()->size);
				records.Push(record);

				high_addr = start_address + record_start_addr;
				high_addr += (sizeof(NamedDataHeader) + HEADER.Get()->size);
				high_addr -= READ_SIZE;
				
				b_start = false;
				HEADER.Reset();
				break;
			}
		}
	}
}

void NamedSpace::Delete(const StaticText32& _name)
{
	Free(_name);
}
