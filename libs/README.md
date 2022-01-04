0.install build essential
------
sudo apt-get install build-essential

1.install libudev-dev
------
sudo apt-get install libudev-dev

2.download usb info
------
download libusb-1.0.X from https://libusb.info/

3.build libusb
------

```bash
./configure
make
sudo make install
```
