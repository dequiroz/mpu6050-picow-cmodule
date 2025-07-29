#include "py/obj.h"
#include "py/runtime.h"
#include "mpu6050_dmp.h"
#include "py/stream.h"

static mp_obj_t imu_init(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    enum { ARG_i2c, ARG_sda, ARG_scl };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_i2c, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_sda, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 20} },
        { MP_QSTR_scl, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 21} },
    };

    mp_arg_val_t parsed_args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, args, kwargs, MP_ARRAY_SIZE(allowed_args), allowed_args, parsed_args);

    uint8_t i2c_id = parsed_args[ARG_i2c].u_int;
    uint sda_pin = parsed_args[ARG_sda].u_int;
    uint scl_pin = parsed_args[ARG_scl].u_int;

    if (!imu_dmp_init(i2c_id, sda_pin, scl_pin)) {
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("MPU6050 init failed"));
    }

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_KW(imu_init_obj, 0, imu_init);


static mp_obj_t imu_get_angles() {
    float yaw, pitch, roll;
    if (!imu_dmp_read(&yaw, &pitch, &roll))
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("Failed to read DMP data"));
    
    mp_obj_t tuple[3] = {
        mp_obj_new_float(yaw),
        mp_obj_new_float(pitch),
        mp_obj_new_float(roll)
    };
    return mp_obj_new_tuple(3, tuple);
}
static MP_DEFINE_CONST_FUN_OBJ_0(imu_get_angles_obj, imu_get_angles);

static mp_obj_t imu_get_yaw() {
    float yaw, pitch, roll;
    if (!imu_dmp_read(&yaw, &pitch, &roll))
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("Failed to read DMP data"));
    return mp_obj_new_float(yaw);
}
static MP_DEFINE_CONST_FUN_OBJ_0(imu_get_yaw_obj, imu_get_yaw);

static mp_obj_t imu_get_pitch() {
    float yaw, pitch, roll;
    if (!imu_dmp_read(&yaw, &pitch, &roll))
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("Failed to read DMP data"));
    return mp_obj_new_float(pitch);
}
static MP_DEFINE_CONST_FUN_OBJ_0(imu_get_pitch_obj, imu_get_pitch);

static mp_obj_t imu_get_roll() {
    float yaw, pitch, roll;
    if (!imu_dmp_read(&yaw, &pitch, &roll))
        mp_raise_msg(&mp_type_RuntimeError, MP_ERROR_TEXT("Failed to read DMP data"));
    return mp_obj_new_float(roll);
}
static MP_DEFINE_CONST_FUN_OBJ_0(imu_get_roll_obj, imu_get_roll);

// Register the module
static const mp_rom_map_elem_t imu_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&imu_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_angles), MP_ROM_PTR(&imu_get_angles_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_yaw), MP_ROM_PTR(&imu_get_yaw_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_pitch), MP_ROM_PTR(&imu_get_pitch_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_roll), MP_ROM_PTR(&imu_get_roll_obj) },
};
static MP_DEFINE_CONST_DICT(imu_module_globals, imu_module_globals_table);

const mp_obj_module_t imu_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&imu_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_imu, imu_module);

