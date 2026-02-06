/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/* Implémentation de ERGO-L https://ergol.org + Arsenik https://github.com/OneDeadKey/arsenik
*  pour un PC sous Windows configuré en azerty.
*
*  Permet d’utiliser Ergo-l sur PC Windows sur lequel on ne peut pas
*  installer les drivers ergo-l https://github.com/Nuclear-Squid/ergol/releases/download/ergol-v1.0.0/ergol_kbd.exe
*  et Kanata https://github.com/jtroo/kanata pour Arsenik https://github.com/OneDeadKey/arsenik/tree/main/kanata
*
*  Utilise la lib azerty de Parinacota (merci à lui) https://github.com/parinacota/qmk_firmware/tree/master/keyboards/unibody36tp/lib
*  et l’implémentation QMK de Arsenik https://github.com/OneDeadKey/arsenik/tree/main/qmk
*
*  • Les layers Windows fournis par Keychron restent inchangés
*  • Les Layers Mac sont remplacés par ERGO-L + Arsenik
*
*  Cela permet de rebasculer le clavier en configuration Windows
*  standard avec le switch physique orange sur la tranche arrière du clavier
*  Mac = Ergo-l pour Windows en Azerty
*  Win = Windows standard
*/

/*
    Il y’a le code nécessaire en principe pour Linux et Mac
    Mais il faudrait le revoir, je ne m’en suis pas du tout occupé.
    Ça ne fonctionne probablenment pas en l’état:
    • J’ai ajouté des caractères supplémentaires par rapport à
    la lib azerty d’origine sans me soucier que cela fonctionne
    pour Mac ou Linux.
    • Je ne me suis pas non plus occupé du tout de la détection
    automatique de l’OS…
*/

/*
    Il manque des choses non essentielles comme:
    • le layer SHIFT+ALTGR est très incomplet (≤,≥,≠ par exemple)
    • les touches mortes ^ ' ` ~ (mais la 1DK normale fonctionne parfaitement pour â,ä,à,ñ par exemple)
    • le layer pour la touche morte secondaire sur la touche G (lettes grecques)
    • l’espace insécable fine (SHIFT+SPC)
    • et problablement d’autres choses
    Essentiellement parceque je n’arrive pas à gérer l’unicode, ou parceque je n’ai pas cherché
    à trop creuser (touches mortes accents) car intérêt est limité par rapport à l’effort à fournir.
*/

#include QMK_KEYBOARD_H
#include "keychron_common.h"

#include <lib/emul_azerty/emul_azerty.h>
#include <lib/arsenik/arsenik_config.h>
#include <lib/arsenik/arsenik.h>
#include "os_detection.h"

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    FN1,
    _SHF,
    _SHF2,
    _SYM,
    _SSYM,
    _1DK,
    _SDK,
    _num_nav,
    _Snum_nav,
    FUNC
};

enum custom_keycodes {
    MA_TOBASE=NEW_SAFE_RANGE,
    MA_CMD_ALT_TAB,
    //FUNCTION LAYER
    KB_MICOFF,
    KB_PSCR,
    LED_UP,
    LED_DOWN,
    HAPT_INC,
    HAPT_DEC,
    HAPT_TGGL,
    LED_FR,
    LED_EU,
    AZ_LINUX,

};

#define FN_MAC MO(MAC_FN)
#define FN_WIN MO(WIN_FN)

// ########### TAP DANCE #############
typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

/*
void keyboard_post_init_user(void) {
//    leds_turn_off();
    emul_keyboard_post_init_user();
}
*/

bool process_detected_host_os_kb(os_variant_t detected_os) {
    //leds_on_for_at(500,HS_WHITE,LED_RIGHT);
    return true;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// les layers windows ne sont pas modifiés.
// les layers MAC et tous les layers additonnels sont utilisés pour ERGO-L.
// cela permet de remettre le clavier en configuration standard windows en
// sélectionnant le mode windows avec le switch physique orange sur la tranche arrière du clavier.
    [MAC_BASE] = LAYOUT_ansi_61(
        KC_ESC,   AZ_1,     AZ_2,     AZ_3,     AZ_4,     AZ_5,     AZ_6,     AZ_7,     AZ_8,     AZ_9,     AZ_0,     AZ_SLSH,  KC_EQL,   KC_BSPC,
        KC_TAB,   AZ_Q,     AZ_C,     AZ_O,     AZ_P,     AZ_W,     AZ_J,     AZ_M,     AZ_D,     OSL(_1DK),AZ_Y,     AZ_LBRC,  AZ_RBRC,  AZ_BSLH,
        KC_CAPS,  AZ_A,     KC_SS,    KC_EE,    KC_NN,    AZ_F,     AZ_L,     KC_RR,    KC_TT,    KC_II,    LT(FUNC,KC_U),      AZ_QUOT,   KC_ENT,
        LM(_SHF, MOD_LSFT), AZ_Z,     AZ_X,     AZ_MINS,  AZ_V,     AZ_B,     AZ_DOT,   AZ_H,     AZ_G,     AZ_COMM,  AZ_K,     LM(_SHF, MOD_LSFT),
        KC_LCTL,  KC_LGUI,  LT(_SHF2,KC_BSPC),                  LT(_num_nav, KC_SPC),   LT(_SYM,KC_ENT),/*MO(_SYM),*/ FN_MAC,   MO(FN1),  KC_RCTL),

    [MAC_FN] = LAYOUT_ansi_61(
        KC_GRV,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  KC_INS,   KC_PGUP,  KC_HOME,  _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  KC_UP,    KC_SNAP,  KC_PGDN,  KC_END,             _______,
        _______,            UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_DEL,             _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_61(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  FN_WIN,   MO(FN1),  KC_RCTL),

    [WIN_FN] = LAYOUT_ansi_61(
        KC_GRV,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  KC_INS,   KC_PGUP,  KC_HOME,  _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  KC_UP,    KC_SNAP,  KC_PGDN,  KC_END,             _______,
        _______,            UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_DEL,             _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______),

    [FN1] = LAYOUT_ansi_61(
        KC_TILD,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,
        _______,  UG_NEXT,  UG_VALU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        UG_TOGG,  _______,  UG_VALD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______),


    [_SHF] = LAYOUT_ansi_61(
        _______, AZ_EURO,  AZ_LQUOTFR,AZ_RQUOTFR,AZ_DLRS, AZ_PERC, AZ_CIR,  AZ_EPERL,  AZ_ASTER,  AZ_CROI,   AZ_AROB,   AZ_UNDER,AZ_PLUS, _______,
        _______, _______,  _______,   _______,   _______, _______, _______, _______,   _______,   AZ_EXCL,   _______,   AZ_LCBR, AZ_RCBR, AZ_BSLH,
        _______, _______,  _______,   _______,   _______, _______, _______, _______,   _______,   _______,   _______,   AZ_DQUO,          _______,
        _______,           _______,   _______,   AZ_COMM, _______, _______, AZ_COLN,   _______,   _______,   AZ_SCLN,   _______,          _______,
        _______, _______,  _______,                                _______,                                  _______,   _______, _______, _______),
// _SHF2 sert pour la touche LALT qui fait SHIFT en hold pour ARSENIK
    [_SHF2] = LAYOUT_ansi_61(
        _______, AZ_EURO,   AZ_LQUOTFR, AZ_RQUOTFR, AZ_DLRS,  AZ_PERC,      AZ_CIR,    AZ_EPERL,  AZ_ASTER,  AZ_CROI,   AZ_AROB,   AZ_UNDER,AZ_PLUS,_______,
        _______, RSFT(AZ_Q),RSFT(AZ_C), RSFT(AZ_O), RSFT(AZ_P),RSFT(AZ_W),  RSFT(AZ_J),RSFT(AZ_M),RSFT(AZ_D),AZ_EXCL,   RSFT(AZ_Y),AZ_LCBR, AZ_RCBR,AZ_BSLH,
        _______, RSFT(AZ_A),RSFT(AZ_S), RSFT(AZ_E), RSFT(AZ_N),RSFT(AZ_F),  RSFT(AZ_L),RSFT(AZ_R),RSFT(AZ_T),RSFT(AZ_I),RSFT(AZ_U),AZ_DQUO,         _______,
        _______, RSFT(AZ_Z),RSFT(AZ_X), RSFT(AZ_COMM),RSFT(AZ_V),RSFT(AZ_B),AZ_COLN,   RSFT(AZ_H),RSFT(AZ_G),AZ_SCLN,   RSFT(AZ_K)         ,_______,
        _______,  _______,  _______,                              LT(_Snum_nav, KC_SPC/*AZ_INSEC*/),           LT(_SSYM,KC_ENT),   _______,_______, _______),

    [_SYM] = LAYOUT_ansi_61(
        KC_ESC,   AZ_SUP1,  AZ_SUP2,  AZ_SUP3,  KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,   AZ_CIR,   AZ_LABK,  AZ_RABK,  AZ_DLR,   AZ_PERC,  AZ_AT,    AZ_AMPR,  AZ_ASTR,  AZ_QUOT,  AZ_GRV,   KC_LBRC,  KC_RBRC,  KC_BSLS,
        KC_CAPS,  AZ_LCBR,  AZ_LPRN,  AZ_RPRN,  AZ_RCBR,  AZ_EQL,   AZ_BSLH,  AZ_PLUS,  AZ_MINS,  AZ_SLSH,  AZ_DQUO,  _______,            KC_ENT,
        LM(_SSYM,MOD_LSFT),AZ_TIL,AZ_LBRC,AZ_RBRC,AZ_UNDS,AZ_HASH,  AZ_PIPE,  AZ_EXCL,  KC_COMM,  AZ_COLN,  RSFT(AZ_COMM),      LM(_SSYM,MOD_LSFT),
        KC_LCTL,  KC_LGUI,  LT(_SSYM,KC_BSPC),                                KC_SPC,                       _______,   FN_WIN,  MO(FN1),  KC_RCTL),

    // je n’ai pas réussi à gérer les caractères unicodes comme ≤,≥,≃,≠, donc j’ai pris un peu de liberté (☺, ♪, ♫, ←, →, ↔)
    // j’ai respecté la spec Ergo-l pour l’espace insécable, (Altgr+Shift+SPC), mais on ne peut pas l’atteindre facilement…
    // il faut utiliser le LSFT normal, pas possible avec ARSENIK (ALT+ALTGR+SPC), il faudrait 3 pouces…
    [_SSYM] = LAYOUT_ansi_61(
        _______, _______,  _______,   _______,   _______, _______, _______,  _______,  _______,   _______,   _______,   _______, _______, _______,
        _______, AZ_DARROW,AZ_LARROW, AZ_RARROW, AZ_HAPPY, AZ_PERM, AZ_NOTE2,  AZ_NOTE,  AZ_MULTIP,_______,  _______,   _______, _______, _______,
        _______, _______,  _______,   _______,   _______, _______, _______,AZ_PLUSMOINS,AZ_MACRON,AZ_DIVIS,  _______,   _______,          _______,
        _______,           _______,   _______,   _______, _______, AZ_BBARE, _______,  _______,   _______,   _______,   _______,          _______,
        _______, _______,  _______,                                AZ_INSEC,                                 _______,   _______, _______, _______),

    [_1DK] = LAYOUT_ansi_61(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     AZ_PARAG, AZ_PIEDMOUCHE,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,   AZ_ACIR,  AZ_CCED,  AZ_OE,    AZ_OCIR,  AZ_DEG,   AZ_SECT,  AZ_MICR,  AZ_UNDS,  AZ_DIAE,  AZ_UCIR,  KC_LBRC,  KC_RBRC,  KC_BSLS,
        KC_CAPS,  AZ_AGRV,  AZ_EACU,  AZ_EGRV,  AZ_ECIR,  AZ_NTILD, AZ_LPRN,  AZ_RPRN,  AZ_ICIR,  AZ_ITRE,  AZ_UGRV,  KC_QUOT,             KC_ENT,
        OSL(_SDK),AZ_AE,    AZ_ESZET, AZ_QCADR, AZ_SCADR, AZ_CADR,  AZ_ETC,   AZ_https, KC_G,     AZ_MDOT,  AZ_COPYR,                    OSL(_SDK),
        TO(MAC_BASE),KC_LGUI, LT(_SDK,KC_BSPC),                               AZ_QUOTFR,                        MO(_SYM), FN_WIN, MO(FN1),KC_RCTL),

    [_SDK] = LAYOUT_ansi_61(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,  KC_BSPC,
        KC_TAB,   AZ_ACIRM, AZ_CCEDM, AZ_OEM,   AZ_OCIRM, AZ_BULLET,XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  AZ_UCIRM, XXXXXXX,  XXXXXXX, XXXXXXX,
        KC_CAPS,  AZ_AGRVM, AZ_EACUM, AZ_EGRVM, AZ_ECIRM, AZ_NTILDM,XXXXXXX,  XXXXXXX,  AZ_ICIRM, AZ_ITREM, AZ_UGRVM, XXXXXXX,            KC_ENT,
        XXXXXXX,  AZ_AEM,   AZ_SESZET, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, AZ_BULLET,AZ_RGSTRD,                    XXXXXXX,
        TO(MAC_BASE),KC_LGUI,XXXXXXX,                               KC_SPC,                                  XXXXXXX, XXXXXXX,  XXXXXXX, KC_RCTL),
    // Idéalement, on pourrait ajouter un [_2DK] et un [_S2DK] pour les caractères grecs et grecs majuscules (touche G)
    // mais je n’arrive pas à gérer l’unicode et la limite du nombre de layers est atteinte.

    [_num_nav] = LAYOUT_ansi_61(
        KC_ESC,   G(KC_1),  G(KC_2),  G(KC_3),  G(KC_4),  G(KC_5),  G(KC_6),  G(KC_7),  G(KC_8), G(KC_9), G(KC_0),  _______,  _______,  _______,
        KC_TAB,   KC_TAB,   KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  AS(SLSH), AS(7),    AZ_8,    AZ_9,    _______,  _______,  _______,  _______,
        KC_CAPS,  KC_CAPS,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_PGDN,  AS(MINS), AS(4),    AZ_5,    AZ_6,    AZ_0,     _______,            _______,
        LM(_Snum_nav, MOD_LSFT),C(AS(Z)),KC_VOLD,KC_MUTE,KC_VOLU, S(KC_TAB),AS(COMM),   AZ_1,    AZ_2,    AZ_3,     AS(DOT), LM(_Snum_nav, MOD_LSFT),
        KC_LCTL,  KC_LGUI,  LT(_Snum_nav,KC_DEL),                   KC_SPC,                               KC_ESC,   FN_WIN,  MO(FN1),   KC_RCTL),

    [_Snum_nav] = LAYOUT_ansi_61(
        _______,   _______,   _______,    _______,    _______,    _______,    _______, _______,  _______,   _______,   _______,    _______, _______, _______,
        S(KC_TAB), S(KC_TAB), S(KC_HOME), S(KC_UP),   S(KC_END),  S(KC_PGUP), _______, AZ_EPERL, AZ_ASTER,  AZ_CROI,   _______,    _______, _______, _______,
        _______,   _______,   S(KC_LEFT), S(KC_DOWN), S(KC_RGHT), S(KC_PGDN), _______, AZ_DLRS,  AZ_PERC,   AZ_CIR,    AZ_AROB,    _______,          _______,
        _______,              _______,    _______,    _______,    _______,    _______, _______,  AZ_EURO,   AZ_LQUOTFR,AZ_RQUOTFR, _______,          _______,
        _______, _______,     _______,                            _______,                                  _______,   _______,    _______, _______ ),

    [FUNC] = LAYOUT_ansi_61(
        _______, _______, _______, _______, _______, _______, _______,  _______,  _______,   _______,   _______,   _______, _______, _______,
        _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4, _______, _______,  _______,  _______,   _______,   _______,   _______, _______, _______,
        _______,   KC_F5,   KC_F6,   KC_F7,   KC_F8, _______, _______,  KC_RR,      KC_TT,     KC_II,   _______,   _______,          _______,
        _______,            KC_F9,  KC_F10,  KC_F11,  KC_F12, _______,  _______,  _______,   _______,   _______,   _______,          _______,
        _______, _______,  _______,                                _______,                             _______,   _______, _______, _______),

};

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // Let QMK do its thing on key releases.
    if (!record->event.pressed) {
        return true;
    }

    process_record_keychron_common(keycode, record);
    emul_process_record_user(keycode, record);

return true;

}

// ARSENIK ------------------------------------------------
// 1. Définit à ARSENIK_HRM_TAPPING_TERM (300ms par défaut)
//    le délai avant que les HRM s’activent sans modifier le
//    TAPPING_TERM par défaut (200ms) des autres touches.
// 2. Permet que les touches alt(shift/bckspc) et altgr(altgr/enter)
//    passent en hold seulement après TAPPING_TERM (200ms)
//    mais aussi qu’elles soient considérées en hold dès qu’une autre touche est
//    pressée sans devoir attendre TAPPING_TERM.

// pour que ça fonctionne, ajouter
// #define TAPPING_TERM_PER_KEY
// #define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// dans keyboards\keychron\k9_max\ansi\rgb\config.h
// et en début de ce fichier (pas sûr qu’en début de ce fichier ce soit indispensable)

static inline bool tap_keycode_used_in_text(uint16_t keycode) {
    // We can’t make assumptions on curstom keycodes
    if (keycode >= SAFE_RANGE) return false;

    // Remove "quantum" part of the keycode to get the action on tap.
    const uint16_t tap_keycode = keycode & 0xff;
    // `tap_keycode <= KC_0` includes all letters and numbers, but also
    // `KC_NO` which is safer to include, since it is commonly used in the
    // keymap as a placeholder for complex actions on tap.
     return (tap_keycode <= KC_0) || (tap_keycode == KC_SPC);
}

// TAPPING_TERM est défini à 200ms par défaut dans quantum/action_taping.h
// ARSENIK_HRM_TAPPING_TERM est défini à 300ms par défaut dans arsenik_config.h
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return tap_keycode_used_in_text(keycode) ? ARSENIK_HRM_TAPPING_TERM : TAPPING_TERM;
}

// nécessite #define HOLD_ON_OTHER_KEY_PRESS_PER_KEY dans keyboards\keychron\k9_max\ansi\rgb\config.h
// et en début de ce fichier (pas sûr qu’en début de ce fichier ce soit indispensable)
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return !tap_keycode_used_in_text(keycode);
}
// FIN ARSENIK --------------------------------------------
