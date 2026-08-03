/**
 * @file TimeCells.hpp
 * @date Jul 11, 2026
 * @author Rakhimov T.
 */

#ifndef INC_TIME_TIMECELLS_HPP_
#define INC_TIME_TIMECELLS_HPP_

#include <cstdint>

#include <Data/IDataCell.hpp>
#include <Time/TimeUtils.hpp>

class CurrentTimeCell : public IDataCell<uint32_t>
{
public:
    CurrentTimeCell(bool mks = false) : mks{ mks }
    { }

    void Set(const uint32_t& value) override { /* Empty */ }

	bool Get(uint32_t& value) const override
    {
        value = GetCurrent();
        return true;
    }

	uint32_t GetOrDefault() const override { return GetCurrent(); }

private:
    bool mks{ false };  ///< Using mks (true) or ms (false)

    uint32_t GetCurrent() const
    {
        return mks ? TimeUtils::GetCurrentMks() : TimeUtils::GetCurrentMs();
    }
};

#endif
