#!/usr/bin/env bash
# =============================================================================
# 环境自检：在 WSL 里跑
#   bash setup/doctor.sh
# 每一项告诉你「过没过」和「没过该干嘛」。
# =============================================================================
set -uo pipefail

REPO="$HOME/workspace/RM-Electrical-Control"
FAIL=0
WARN=0

ok()   { printf '  \033[32m[✓]\033[0m %-34s %s\n' "$1" "${2:-}"; }
bad()  { printf '  \033[31m[✗]\033[0m %-34s %s\n' "$1" "${2:-}"; FAIL=$((FAIL+1)); }
warn() { printf '  \033[33m[!]\033[0m %-34s %s\n' "$1" "${2:-}"; WARN=$((WARN+1)); }

echo
echo "== 1. 编译器 =="
if cmd=$(command -v arm-none-eabi-gcc); then
  ok "arm-none-eabi-gcc" "$(arm-none-eabi-gcc -dumpversion 2>/dev/null) → $cmd"
else
  bad "arm-none-eabi-gcc" "跑 sudo bash setup/install-toolchain.sh"
fi
command -v arm-none-eabi-objcopy >/dev/null && ok "arm-none-eabi-objcopy" "$(command -v arm-none-eabi-objcopy)" \
  || bad "arm-none-eabi-objcopy" "同上"

echo
echo "== 2. 构建系统 =="
if [ -x /usr/bin/cmake ]; then
  ok "cmake" "$(/usr/bin/cmake --version | head -1)"
elif command -v cmake >/dev/null; then
  warn "cmake 不在 /usr/bin" "$(command -v cmake) —— 能编，但建议 sudo 装一份"
else
  bad "cmake" "sudo bash setup/install-toolchain.sh"
fi
if command -v ninja >/dev/null; then
  ok "ninja" "$(ninja --version 2>/dev/null)"
else
  bad "ninja" "sudo bash setup/install-toolchain.sh"
fi

echo
echo "== 3. 调试 / 烧录 =="
if command -v openocd >/dev/null; then
  ok "openocd" "$(openocd --version 2>&1 | head -1 | sed 's/Open On-Chip Debugger //')"
else
  bad "openocd" "sudo bash setup/install-toolchain.sh"
fi
if command -v gdb-multiarch >/dev/null; then
  ok "gdb-multiarch" "$(gdb-multiarch --version | head -1 | sed 's/GNU gdb //')"
else
  bad "gdb-multiarch" "sudo bash setup/install-toolchain.sh"
fi

echo
echo "== 4. 权限 =="
if [ -f /etc/udev/rules.d/70-stlink.rules ]; then
  ok "ST-Link udev 规则" "/etc/udev/rules.d/70-stlink.rules"
else
  bad "ST-Link udev 规则" "sudo bash setup/install-toolchain.sh"
fi
if id -nG | grep -qw plugdev; then
  ok "plugdev 组" "已加入"
else
  warn "plugdev 组" "sudo usermod -aG plugdev \$USER，然后重开终端"
fi

echo
echo "== 5. 硬件链路 =="
if command -v lsusb >/dev/null; then
  if lsusb 2>/dev/null | grep -qi '0483:'; then
    ok "WSL 能看到 ST-Link" "$(lsusb | grep -i 0483 | head -1 | cut -c1-70)"
  else
    bad "WSL 看不到 ST-Link" "Windows 管理员跑 setup/windows-usbipd.ps1"
  fi
else
  warn "lsusb 未安装" "sudo apt install usbutils"
fi
if [ -d "/mnt/c/Program Files/usbipd-win" ]; then
  ok "usbipd-win（Windows 侧）" "已安装"
else
  warn "usbipd-win（Windows 侧）" "winget install --exact --id dorssel.usbipd-win"
fi

echo
echo "== 6. 工程 =="
if [ -f "$REPO/CMakePresets.json" ]; then
  ok "构建系统文件" "$REPO"
else
  bad "构建系统文件" "仓库里缺 CMakePresets.json"
fi
for t in lab1 lab2 gm6020; do
  if [ -f "$REPO/build/$t.elf" ]; then
    ok "$t.elf" "$(stat -c%s "$REPO/build/$t.elf" 2>/dev/null) bytes"
  else
    warn "$t.elf 还没编出来" "cmake --preset debug && cmake --build build"
  fi
done

echo
if [ "$FAIL" -eq 0 ] && [ "$WARN" -eq 0 ]; then
  printf '\033[32m全部通过，可以直接 F5 调试。\033[0m\n\n'
elif [ "$FAIL" -eq 0 ]; then
  printf '\033[33m核心项都在，还有 %d 个提醒项。\033[0m\n\n' "$WARN"
else
  printf '\033[31m还有 %d 个必做项没过（上面标 ✗ 的行），按提示处理。\033[0m\n\n' "$FAIL"
fi

exit $(( FAIL > 0 ))
