`ifndef __memory_h_
`define __memory_h_

`define WIDTH 'h20
`define  SIZE 'h200000

/* general registers */
`define expansion_1_base_address 'h1F801000
`define expansion_2_base_address 'h1F801004
`define expansion_1              'h1F800008
`define expansion_3              'h1F80000C
`define bios_rom                 'h1F800010
`define spu_delay                'h1F800014
`define cdrom_delay              'h1F800018
`define expansion_2              'h1F80001C
`define com_delay                'h1F800020

/* interrupt control registers */'h
`define i_stat                   'h1F801070
`define i_mask                   'h1F801074

/* dma registers */
`define dma0_mdec_in_madr        'h1F801080
`define dma0_mdec_in_brc         'h1F801084
`define dma0_mdec_in_chcr        'h1F801088
`define dma1_mdec_out_madr       'h1F801090
`define dma1_mdec_out_brc        'h1F801094
`define dma1_mdec_out_chcr       'h1F801098
`define dma2_gpu_madr            'h1F8010A0
`define dma2_gpu_brc             'h1F8010A4
`define dma2_gpu_chcr            'h1F8010A8
`define dma3_cdrom_madr          'h1F8010B0
`define dma3_cdrom_brc           'h1F8010B4
`define dma3_cdrom_chcr          'h1F8010B8
`define dma4_spu_madr            'h1F8010C0
`define dma4_spu_brc             'h1F8010C4
`define dma4_spu_chcr            'h1F8010C8
`define dma5_pio_madr            'h1F8010D0
`define dma5_pio_brc             'h1F8010D4
`define dma5_pio_chcr            'h1F8010D8
`define dma6_otc_madr            'h1F8010E0
`define dma6_otc_brc             'h1F8010E4
`define dma6_otc_chcr            'h1F8010E8
`define dpcr                     'h1F8010F0
`define dicr                     'h1F8010F4

/* timer registers */
`define timer_0_current_counter  'h1F801100
`define timer_0_mode             'h1F801104
`define timer_0_target           'h1F801108
`define timer_1_current_counter  'h1F801110
`define timer_1_mode             'h1F801114
`define timer_1_target           'h1F801118
`define timer_2_current_counter  'h1F801120
`define timer_2_mode             'h1F801124
`define timer_2_target           'h1F801128

/* gpu registers */
`define gp0_gpu_read             'h1F801810
`define gp1_gpu_stat             'h1F801814

/* cache control */
`define cache_control            'hFFFE0130

`endif // __memory_h_
