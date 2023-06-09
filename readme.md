# Wujian100 on Basys3

在 Basys3 上运行平头哥 Wujian100 SoC，并添加简单外设。

为数电课设计：[实验视频](https://www.bilibili.com/video/BV1MV4y1S7Ph)

## 项目结构

- doc：相关文档。
- sdk：Wujian100 SDK。添加的实验代码位于：`\sdk\projects\Labs\`。
- base：经过裁剪的 Wujian100。在源码基础上做了如下修改：
  - 删除了 DMA 模块。删除了第三个 DATA SRAM，其地址空间为：0x2002_0000~0x2002_FFFF。因此，修改了 CDK 项目链接脚本 `sdk\board\wujian100_open_evb\gcc_csky.ld` 的地址空间。
  - 修改了约束文件：映射了 GPIO，USART，JTAG，RST_N 等信号。其中系统的低有效复位键映射到了拨码开关 SW15 上，SW15 断开时系统复位。
  - 添加了 pll 分频器，将板载 100MHz 时钟分频为 20MHz，以适用于 Wujian100 SoC。
  - 编写了 tcl 脚本，用于忽略综合和比特流生成时的警告。
- 3led：在 base 基础上添加了三色灯控制器 `myio`。



