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
    // Body
    vlSelf->top__DOT__segs[0U] = 0xfdU;
    vlSelf->top__DOT__segs[1U] = 0x60U;
    vlSelf->top__DOT__segs[2U] = 0xdaU;
    vlSelf->top__DOT__segs[3U] = 0xf2U;
    vlSelf->top__DOT__segs[4U] = 0x66U;
    vlSelf->top__DOT__segs[5U] = 0xb6U;
    vlSelf->top__DOT__segs[6U] = 0xbeU;
    vlSelf->top__DOT__segs[7U] = 0xe0U;
    vlSelf->top__DOT__new_in = (1U & VL_REDXOR_32((0x1dU 
                                                   & (IData)(vlSelf->top__DOT__q))));
    vlSelf->seg1 = vlSelf->top__DOT__segs[(0xfU & ((IData)(vlSelf->top__DOT__q) 
                                                   >> 4U))];
    vlSelf->seg2 = vlSelf->top__DOT__segs[(0xfU & (IData)(vlSelf->top__DOT__q))];
}

void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vclklast__TOP__clk = vlSelf->clk;
}

void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    // Body
    Vtop___024root___settle__TOP__1(vlSelf);
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
    vlSelf->seg1 = 0;
    vlSelf->seg2 = 0;
    vlSelf->top__DOT__new_in = 0;
    vlSelf->top__DOT__q = 0;
    for (int __Vi0=0; __Vi0<16; ++__Vi0) {
        vlSelf->top__DOT__segs[__Vi0] = 0;
    }
}
