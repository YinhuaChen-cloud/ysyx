// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop___024root.h"
#include "Vtop__Syms.h"

//==========


void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf);

Vtop___024root::Vtop___024root(const char* _vcname__)
    : VerilatedModule(_vcname__)
 {
    // Reset structure values
    Vtop___024root___ctor_var_reset(this);
}

void Vtop___024root::__Vconfigure(Vtop__Syms* _vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->vlSymsp = _vlSymsp;
}

Vtop___024root::~Vtop___024root() {
}

void Vtop___024root___settle__TOP__1(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___settle__TOP__1\n"); );
    // Variables
    CData/*0:0*/ top__DOT__my_vga_ctrl__DOT__h_valid;
    SData/*9:0*/ top__DOT__h_addr;
    IData/*31:0*/ top__DOT__i;
    // Body
    VL_WRITEF("col_remainder\n");
    vlSelf->VGA_HSYNC = (0x60U < (IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__x_cnt));
    vlSelf->VGA_VSYNC = (2U < (IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__y_cnt));
    vlSelf->top__DOT__my_vga_ctrl__DOT__v_valid = (
                                                   (0x23U 
                                                    < (IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__y_cnt)) 
                                                   & (0x203U 
                                                      >= (IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__y_cnt)));
    top__DOT__my_vga_ctrl__DOT__h_valid = ((0x90U < (IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__x_cnt)) 
                                           & (0x310U 
                                              >= (IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__x_cnt)));
    if (vlSelf->top__DOT__my_vga_ctrl__DOT__v_valid) {
        vlSelf->top__DOT__v_addr = (0x3ffU & ((IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__y_cnt) 
                                              - (IData)(0x24U)));
        vlSelf->VGA_BLANK_N = ((IData)(top__DOT__my_vga_ctrl__DOT__h_valid) 
                               & 1U);
    } else {
        vlSelf->top__DOT__v_addr = 0U;
        vlSelf->VGA_BLANK_N = 0U;
    }
    top__DOT__h_addr = ((IData)(top__DOT__my_vga_ctrl__DOT__h_valid)
                         ? (0x3ffU & ((IData)(vlSelf->top__DOT__my_vga_ctrl__DOT__x_cnt) 
                                      - (IData)(0x91U)))
                         : 0U);
    vlSelf->top__DOT__col = 0U;
    vlSelf->top__DOT__col_remainder = 0U;
    top__DOT__i = 0U;
    while (VL_GTS_III(1,32,32, 0x46U, top__DOT__i)) {
        if (((((IData)(9U) * top__DOT__i) <= (IData)(top__DOT__h_addr)) 
             & ((IData)(top__DOT__h_addr) < ((IData)(9U) 
                                             + ((IData)(9U) 
                                                * top__DOT__i))))) {
            vlSelf->top__DOT__col = (0x7fU & top__DOT__i);
        }
        if ((((IData)(0x46U) * top__DOT__i) == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 0U;
        }
        if ((((IData)(1U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 1U;
        }
        if ((((IData)(2U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 2U;
        }
        if ((((IData)(3U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 3U;
        }
        if ((((IData)(4U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 4U;
        }
        if ((((IData)(5U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 5U;
        }
        if ((((IData)(6U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 6U;
        }
        if ((((IData)(7U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 7U;
        }
        if ((((IData)(8U) + ((IData)(0x46U) * top__DOT__i)) 
             == (IData)(top__DOT__h_addr))) {
            vlSelf->top__DOT__col_remainder = 8U;
        }
        top__DOT__i = ((IData)(1U) + top__DOT__i);
    }
}

void Vtop___024root___initial__TOP__2(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___initial__TOP__2\n"); );
    // Variables
    VlWide<6>/*191:0*/ __Vtemp2;
    // Body
    __Vtemp2[0U] = 0x2e747874U;
    __Vtemp2[1U] = 0x666f6e74U;
    __Vtemp2[2U] = 0x7667615fU;
    __Vtemp2[3U] = 0x7263652fU;
    __Vtemp2[4U] = 0x65736f75U;
    __Vtemp2[5U] = 0x72U;
    VL_READMEM_N(true, 12, 4096, 0, VL_CVT_PACK_STR_NW(6, __Vtemp2)
                 ,  &(vlSelf->top__DOT__dotmatrix), 0
                 , ~0ULL);
    vlSelf->top__DOT__vmem[0U] = 0x42U;
}

void Vtop___024root___settle__TOP__5(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___settle__TOP__5\n"); );
    // Variables
    CData/*7:0*/ top__DOT__asciidata;
    IData/*23:0*/ top__DOT__vga_data;
    // Body
    top__DOT__asciidata = ((0x8bfU >= (((IData)(vlSelf->top__DOT__col) 
                                        << 5U) | (0x1fU 
                                                  & ((IData)(vlSelf->top__DOT__v_addr) 
                                                     >> 4U))))
                            ? vlSelf->top__DOT__vmem
                           [(((IData)(vlSelf->top__DOT__col) 
                              << 5U) | (0x1fU & ((IData)(vlSelf->top__DOT__v_addr) 
                                                 >> 4U)))]
                            : 0U);
    vlSelf->top__DOT__font[0U] = vlSelf->top__DOT__dotmatrix
        [((IData)(top__DOT__asciidata) << 4U)];
    vlSelf->top__DOT__font[1U] = vlSelf->top__DOT__dotmatrix
        [(1U | ((IData)(top__DOT__asciidata) << 4U))];
    top__DOT__vga_data = (((0xbU >= (IData)(vlSelf->top__DOT__col_remainder)) 
                           & (vlSelf->top__DOT__font
                              [(0xfU & (IData)(vlSelf->top__DOT__v_addr))] 
                              >> (IData)(vlSelf->top__DOT__col_remainder)))
                           ? 0xfffU : 0U);
    vlSelf->VGA_R = (0xffU & (top__DOT__vga_data >> 0x10U));
    vlSelf->VGA_G = (0xffU & (top__DOT__vga_data >> 8U));
    vlSelf->VGA_B = (0xffU & top__DOT__vga_data);
}

void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    // Body
    Vtop___024root___initial__TOP__2(vlSelf);
    vlSelf->__Vclklast__TOP__clk = vlSelf->clk;
    vlSelf->__Vclklast__TOP__rst = vlSelf->rst;
}

void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    // Body
    Vtop___024root___settle__TOP__1(vlSelf);
    Vtop___024root___settle__TOP__5(vlSelf);
}

void Vtop___024root___final(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___final\n"); );
}

void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = 0;
    vlSelf->rst = 0;
    vlSelf->VGA_VSYNC = 0;
    vlSelf->VGA_HSYNC = 0;
    vlSelf->VGA_BLANK_N = 0;
    vlSelf->VGA_R = 0;
    vlSelf->VGA_G = 0;
    vlSelf->VGA_B = 0;
    for (int __Vi0=0; __Vi0<4096; ++__Vi0) {
        vlSelf->top__DOT__dotmatrix[__Vi0] = 0;
    }
    for (int __Vi0=0; __Vi0<2240; ++__Vi0) {
        vlSelf->top__DOT__vmem[__Vi0] = 0;
    }
    vlSelf->top__DOT__col = 0;
    vlSelf->top__DOT__col_remainder = 0;
    for (int __Vi0=0; __Vi0<16; ++__Vi0) {
        vlSelf->top__DOT__font[__Vi0] = 0;
    }
    vlSelf->top__DOT__v_addr = 0;
    vlSelf->top__DOT__my_vga_ctrl__DOT__x_cnt = 0;
    vlSelf->top__DOT__my_vga_ctrl__DOT__y_cnt = 0;
    vlSelf->top__DOT__my_vga_ctrl__DOT__v_valid = 0;
}
