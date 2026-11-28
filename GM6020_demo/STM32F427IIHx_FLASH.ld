/*
 * STM32F427IIHx 链接脚本（2MB Flash / 192KB RAM / 64KB CCM）
 *
 * 依据 Keil 工程里的 IROM/IRAM 配置换算而来：
 *   IROM  0x08000000 - 0x081FFFFF   2048K
 *   IRAM  0x20000000 - 0x2002FFFF    192K
 *   IRAM2 0x10000000 - 0x1000FFFF     64K  (CCM，只能放数据，不能放代码/常量)
 *
 * 注意：CCM 区（.ccmram）不会被启动代码自动初始化，
 *       如果需要，必须在 main() 之前自己从 Flash 拷贝 _siccmram -> _sccmram。
 */
ENTRY(Reset_Handler)

/* 栈顶 = RAM 末尾 */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* 堆/栈最小保留空间（供 _sbrk 与链接期检查使用） */
_Min_Heap_Size  = 0x200;   /* 512 B  */
_Min_Stack_Size = 0x400;   /* 1024 B */

MEMORY
{
  RAM    (xrw) : ORIGIN = 0x20000000, LENGTH = 192K
  CCMRAM (rw)  : ORIGIN = 0x10000000, LENGTH = 64K
  FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
}

SECTIONS
{
  /* 中断向量表必须落在 Flash 最前面 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  .ARM.extab :
  {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >FLASH

  .ARM :
  {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >FLASH

  .preinit_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >FLASH

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >FLASH

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >FLASH

  /* .data 的初始值存在 Flash 里，启动代码运行期拷到 RAM */
  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT> FLASH

  . = ALIGN(4);
  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM

  . = ALIGN(4);
  .ccmram :
  {
    . = ALIGN(4);
    _sccmram = .;
    *(.ccmram)
    *(.ccmram*)
    . = ALIGN(4);
    _eccmram = .;
  } >CCMRAM AT> FLASH

  /* 链接期检查：RAM 是否还塞得下最小的堆 + 栈 */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
