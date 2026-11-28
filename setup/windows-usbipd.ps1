<#
.SYNOPSIS
    把 ST-Link 从 Windows 直通进 WSL（USB/IP）。

.DESCRIPTION
    WSL2 默认看不到 USB 设备。本脚本会：
      1. 检查 / 安装 usbipd-win
      2. 自动找出 ST-Link（VID 0483）的 BUSID
      3. bind（只需一次）并 attach 到 WSL

    必须以管理员身份运行。板子重新插拔或 wsl --shutdown 之后重跑一次即可。

.EXAMPLE
    右键 PowerShell -> 以管理员身份运行
    pwsh -ExecutionPolicy Bypass -File setup\windows-usbipd.ps1
#>

$ErrorActionPreference = 'Stop'

function Test-Admin {
    $id = [Security.Principal.WindowsIdentity]::GetCurrent()
    (New-Object Security.Principal.WindowsPrincipal $id).IsInRole(
        [Security.Principal.WindowsBuiltInRole]::Administrator)
}

if (-not (Test-Admin)) {
    Write-Host "需要管理员权限。请右键 PowerShell -> 以管理员身份运行，再执行本脚本。" -ForegroundColor Red
    exit 1
}

# ---------- 1. 检查 usbipd ----------
$usbipd = Get-Command usbipd -ErrorAction SilentlyContinue
if (-not $usbipd) {
    Write-Host "未检测到 usbipd-win，开始安装..." -ForegroundColor Yellow
    if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
        Write-Host "没有 winget，请手动下载安装：https://github.com/dorssel/usbipd-win/releases" -ForegroundColor Red
        exit 1
    }
    winget install --exact --id dorssel.usbipd-win `
        --accept-package-agreements --accept-source-agreements
    Write-Host "安装完成，请重开一个管理员 PowerShell 后重新运行本脚本。" -ForegroundColor Yellow
    exit 0
}

Write-Host "usbipd: $($usbipd.Source)" -ForegroundColor Green

# ---------- 2. 找 ST-Link ----------
$lines = & usbipd list 2>&1 | Out-String
Write-Host "`n当前 USB 设备：`n$lines"

# usbipd list 每行形如： 2-1   0483:3748  STM32 STLink      Not shared
$pattern = '^\s*(\d+-\d+)\s+0483:[0-9a-fA-F]{4}\s+(.*)$'
$candidates = @()
foreach ($line in ($lines -split "`r?`n")) {
    if ($line -match $pattern) {
        $candidates += [pscustomobject]@{
            BusId  = $Matches[1]
            Detail = $Matches[2].Trim()
        }
    }
}

if ($candidates.Count -eq 0) {
    Write-Host "`n没找到 ST-Link（VID 0483）。请确认：" -ForegroundColor Red
    Write-Host "  1) 调试器的 USB 线插好了（有些线只能充电不能传数据）"
    Write-Host "  2) 驱动装好了（设备管理器里能看到 STM32 STLink）"
    exit 1
}

if ($candidates.Count -gt 1) {
    Write-Host "`n检测到多个 ST-Link：" -ForegroundColor Yellow
    $candidates | Format-Table -AutoSize | Out-String | Write-Host
    $idx = Read-Host "输入要直通的序号 (0..$($candidates.Count - 1))"
    $target = $candidates[[int]$idx]
} else {
    $target = $candidates[0]
}

Write-Host "`n目标设备：BUSID=$($target.BusId)  $($target.Detail)" -ForegroundColor Green

# ---------- 3. bind + attach ----------
if ($target.Detail -match 'Not shared') {
    Write-Host "执行 bind（只需一次）..."
    & usbipd bind --busid $target.BusId
}

Write-Host "执行 attach 到 WSL..."
& usbipd attach --wsl --busid $target.BusId

Write-Host "`n完成。现在在 WSL 里执行验证：" -ForegroundColor Green
Write-Host "    lsusb | grep -i 0483" -ForegroundColor Cyan
Write-Host "    openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c 'init; targets; exit'" -ForegroundColor Cyan
