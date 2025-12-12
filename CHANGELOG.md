# Changelog

##
- Add Docker support for Z21Sim with noVNC web UI ([#21](https://github.com/ZIMO-Elektronik/Z21/pull/21))
- Add macOS support for Z21Sim ([#21](https://github.com/ZIMO-Elektronik/Z21/pull/21))

## 0.3.6
- Bugfix ESP-IDF example does not compile due to missing `get_socket.*pp` files
- Bugfix ESP-IDF setting `Werror` and Z21 setting `Wpedantic` results in failed build

## 0.3.5
- Add `CommonSettings::ExtFlags` enumeration

## 0.3.4
- Bugfix accessory address encoding still wrong ([#17](https://github.com/ZIMO-Elektronik/Z21/issues/17))

## 0.3.3
- Bugfix accessory address encoding of `LAN_X_CV_POM_ACCESSORY_*` commands is weird ([#15](https://github.com/ZIMO-Elektronik/Z21/issues/15))

## 0.3.2
- Bugfix `LAN_X_LOCO_INFO` DB0 (most significant bits of loco address) ([#11](https://github.com/ZIMO-Elektronik/Z21/issues/11))
- Bugfix `LAN_RAILCOM_GETDATA` used little endian loco address ([#12](https://github.com/ZIMO-Elektronik/Z21/issues/12))

## 0.3.1
- Bugfix `LAN_X_BC_PROGRAMMING_MODE` comes only after `LAN_X_CV_NACK_SC`, `LAN_X_CV_NACK` or `LAN_X_CV_RESULT` ([#7](https://github.com/ZIMO-Elektronik/Z21/issues/7))

## 0.3.0
- Add RailCom interface

## 0.2.0
- Add `LAN_X_SET_LOCO_FUNCTION_GROUP`

## 0.1.1
- Bugfix increase Z21Sim example minimum window size to 1920x900

## 0.1.0
- Initial release