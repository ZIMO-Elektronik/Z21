- When entering the "steering" mode the app seems to flush the UDP buffer by sending 1000 zeros...?
- Currently all examples are IPv4 only
- On Linux system, use socat to listen to UDP stuff:  
  sudo socat UDP4-LISTEN:21105,fork,reuseaddr UDP4:192.168.0.111:21105
- Useful wireshark filter:  
  udp && (ip.dst==192.168.0.111 || ip.src==192.168.0.111)
- I'm somewhat torn between giving programming mode it's own getter/setter or not. On the one hand it's additional overhead and the Z21 app seems to ignore the LAN_X_BC_PROGRAMMING_MODE broadcast anyhow. On the other hand it would most likely simplify the user implementation because then user's wouldn't need to check if they are in service mode when receiving e.g. LAN_X_CV_READ. Then again... should the system even know about anything else but DCC? Ok, let's try something. Let's go one step further in the other direction. Let's remove the trackPower getter! Why should it fail? Because if it fails in case of a short or so, then there is a broadcast for that... if it fails because some other mode is active, just don't receive and execute that command in the first place.
- Color palette generator: https://mokole.com/palette.html

####
Typical CV process for LAN_X_CV_READ:
- LAN_X_CV_READ
- LAN_X_BC_PROGRAMMING_MODE
- LAN_X_CV_RESULT
- CV8 read
- LAN_X_SET_TRACK_POWER_ON

After all service mode read accesses there is a CV8 read (LAN_X_CV_READ->LAN_X_CV_RESULT).
LAN_X_CV_READ and LAN_X_CV_WRITE implicitly turn on track voltage!

Typical CV process for LAN_X_CV_WRITE:
- LAN_X_CV_WRITE
- LAN_X_BC_PROGRAMMING_MODE
- LAN_X_CV_RESULT (ACK with same value)
- LAN_X_SET_TRACK_POWER_ON

Typical CV process for LAN_X_CV_POM_READ_BYTE:
- CV8 read
- LAN_X_CV_POM_READ_BYTE (the actual one)
- LAN_X_CV_RESULT

Typical CV process for LAN_X_CV_POM_WRITE_BYTE:
- CV8 read
- LAN_X_CV_POM_WRITE_BYTE

Prior all POM accesses there is a CV8 read (LAN_X_CV_POM_READ_BYTE->LAN_X_CV_RESULT).
POM accesses do not implicitly turn on track voltage!