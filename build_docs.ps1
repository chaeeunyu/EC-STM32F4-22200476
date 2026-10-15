# 1. Doxygen: headers -> docs/xml
doxygen
if ($LASTEXITCODE -ne 0) { Write-Error "doxygen failed"; exit 1 }

# 2. moxygen: docs/xml -> api-<group>.md
moxygen --anchors --groups --output api-%s.md docs/xml
if ($LASTEXITCODE -ne 0) { Write-Error "moxygen failed"; exit 1 }

# 3. intro.md + api-*.md -> README.md (in this order)
$files = @(
    "intro.md",
    "api-PinNames.md",
    "api-RCC.md",
    "api-GPIO.md",
    "api-SysTick.md",
    "api-EXTI.md",
    "api-Stepper.md",
    "api-USART.md",
    "api-MCU.md"
)

$out = foreach ($f in $files) {
    Get-Content $f -Encoding utf8
    ""   # blank line between files
}
$out | Set-Content README.md -Encoding utf8

Write-Host "README.md updated."