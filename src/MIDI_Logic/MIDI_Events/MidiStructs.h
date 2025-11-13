//
// Created by idad on 12/11/2025.
//

#ifndef MYPROJECT_MIDISTRUCTS_H
#define MYPROJECT_MIDISTRUCTS_H

struct Note {
    uint8_t m_pitch;
    uint8_t m_velocity;
};
struct Program_Cha {
    uint8_t m_program_number;
};
struct Control_Cha {
    uint8_t m_controller;
    uint8_t m_value;
};
struct Meta_Ev {
    uint8_t m_meta_type;
    uint32_t m_v_length;
    std::vector<uint8_t> m_meta_bytes;
};
struct Pitch_Be {
    uint8_t m_lsb_value;
    uint8_t m_msb_value;
};
struct Poly_Af {
    uint8_t m_pressure;
    uint8_t m_pitch;
};
struct Poly_Cha {
    uint8_t m_pressure;
};
struct SysEx {
    std::vector<uint8_t> m_meta_bytes;
};

#endif //MYPROJECT_MIDISTRUCTS_H