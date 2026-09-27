set bat_generator_path=%LETO_PATH%/LetoCore/Utils/fontmap2c.py

python3 %bat_generator_path% base_6x6_ascii.bmp base_6x6_ascii
python3 %bat_generator_path% base_6x6_rus.bmp base_6x6_rus --skip-empty
