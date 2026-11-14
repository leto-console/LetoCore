#!/bin/bash

echo "[LetoCore] Собираем проект..."
set -e

(
    cd "$(dirname "${BASH_SOURCE}")" || exit 1

    ./preset_setup.sh LetoCore linux-debug
    ./preset_setup.sh LetoCore stm32f411xe-debug
)
