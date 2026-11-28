#!/usr/bin/env bash
# =============================================================================
# 检查 VS Code 的 STM32 相关扩展是否装好，缺什么补什么
#   bash setup/install-extensions.sh
#
# 注意：扩展要装在「WSL: Ubuntu」这一侧，请在 VS Code 连上 WSL 之后
#       的集成终端里执行本脚本。
# =============================================================================
set -uo pipefail

EXTENSIONS=(
  "marus25.cortex-debug"     # 调试（OpenOCD + gdb）
  "ms-vscode.cpptools"       # 代码补全 / IntelliSense
  "ms-vscode.cmake-tools"    # CMake 集成
  "ms-vscode.hexeditor"      # 看 .hex / .bin
)

if ! command -v code >/dev/null 2>&1; then
  echo "找不到 code 命令。" >&2
  echo "在 VS Code 里按 Ctrl+Shift+P → 'Shell Command: Install code command in PATH'" >&2
  exit 1
fi

installed="$(code --list-extensions 2>/dev/null | tr '[:upper:]' '[:lower:]')"

for ext in "${EXTENSIONS[@]}"; do
  lower="$(echo "$ext" | tr '[:upper:]' '[:lower:]')"
  if echo "$installed" | grep -qx "$lower"; then
    echo "  [已装] $ext"
  else
    echo "  [安装] $ext"
    code --install-extension "$ext" --force || echo "    安装失败，可在扩展面板手动装"
  fi
done

echo
echo "==> 扩展就绪。若 IntelliSense 报找不到头文件，先跑一次 cmake --preset debug 生成 compile_commands.json"
