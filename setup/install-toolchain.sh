#!/usr/bin/env bash
# =============================================================================
# 一次性的系统依赖安装（需要 sudo）
#   sudo bash setup/install-toolchain.sh
#
# 装完之后系统里会有：
#   arm-none-eabi-gcc  交叉编译器
#   cmake / ninja-build 构建系统
#   openocd             下载 + 调试服务
#   gdb-multiarch       调试器（cortex-debug 用）
# =============================================================================
set -euo pipefail

if [ "$(id -u)" -ne 0 ]; then
  echo "需要 root 权限，请这样执行：  sudo bash setup/install-toolchain.sh" >&2
  exit 1
fi

REAL_USER="${SUDO_USER:-$USER}"

echo "==> 1/3 更新 apt 索引"
apt-get update

echo "==> 2/3 安装工具链"
DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
  build-essential \
  cmake \
  ninja-build \
  git \
  gcc-arm-none-eabi \
  binutils-arm-none-eabi \
  libnewlib-arm-none-eabi \
  gdb-multiarch \
  openocd \
  usbutils

echo "==> 3/3 安装 ST-Link udev 规则（免 root 访问调试器）"
cat > /etc/udev/rules.d/70-stlink.rules <<'RULES'
# ST-Link / ST-Link V2 / V2-1 / V3  —— 让普通用户可以直接用 openocd 访问
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3744", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="374b", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="374d", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="374e", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="374f", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3752", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3753", MODE="0666", GROUP="plugdev", TAG+="uaccess"
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3754", MODE="0666", GROUP="plugdev", TAG+="uaccess"
RULES

udevadm control --reload-rules || true
udevadm trigger || true

# 把用户加进 plugdev，否则 GROUP="plugdev" 这条不生效
if ! id -nG "$REAL_USER" | grep -qw plugdev; then
  usermod -aG plugdev "$REAL_USER" || true
  echo "    已把 $REAL_USER 加入 plugdev 组（需要重新登录 WSL 生效）"
fi

echo
echo "==> 完成。检查一下："
for c in arm-none-eabi-gcc cmake ninja openocd gdb-multiarch; do
  printf '    %-20s %s\n' "$c" "$(command -v "$c" || echo '缺失!')"
done
