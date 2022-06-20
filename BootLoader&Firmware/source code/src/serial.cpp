#include "serial.h"
#include <Arduino.h>

slider_packet_t slider_req, slider_resp;
int slider_tx_pending = 0;

// int checkcom = 0;

uint8_t sliderserial_readreq() {
	static short ptr = 0;
	static uint8_t checksum;
	static bool esc = false;
	uint8_t c;
	while (SerialUSB.available()) {

		// checkcom = SLIDER_CMD_AUTO_SCAN;

		c = SerialUSB.read();
		// SYN
		if (c == 0xff) {
//			 SerialUSB.println("SYN");
			ptr = 0;
			slider_req.syn = c;
			slider_req.cmd = 0; // Unset cmd
			slider_req.size = 0xff; // Unset size
			checksum = 0xff;
			esc = false;
			continue;
		}

		// Escape
		if (esc) {
			esc = false;
			c++;
		} else if (c == 0xfd) {
			esc = true;
			continue;
		}
		// Checksum
		checksum += c;
		// Byte 2 (opcode)
		if (!slider_req.cmd) {
//			 SerialUSB.print("Cmd: ");
//			 SerialUSB.println(c, HEX);
			slider_req.cmd = c;
			continue;
		}
		// Byte 3 (size)
		if (slider_req.size == 0xff) {
//			 SerialUSB.print("Size: ");
//			 SerialUSB.println(c);
			slider_req.size = c;
			ptr = 3;
			continue;
		}
		// Payload Body
		if (ptr >= 128) {
			// Buffer full. Invalidate. Wait for next SYN.
			continue;
		}
		slider_req.data[ptr++] = c;
		if (ptr-3 == slider_req.size+1) {
			// End
			if (checksum == 0) {
//				 SerialUSB.println("OK");
				return slider_req.cmd;
			}
//			 SerialUSB.println("Checksum Error!");
		}
	}
	return 0;
}

void sliderserial_writeresp() {
	static short ptr = 0;
	static uint8_t checksum;
	// Detect new response
	if (!slider_tx_pending) {
		if (slider_resp.cmd) {
			slider_tx_pending = slider_resp.size + 4;
			ptr = 0;
			checksum = 0;
		} else {
			return;
		}
	}
	while (slider_tx_pending) {
		uint8_t c;
		if (slider_tx_pending == 1) {
			c = -checksum;
		} else {
			c = slider_resp.data[ptr];
		}
		checksum += c;
		if ((ptr != 0 && c == 0xff) || c == 0xfd) {
			// Escape required
			if (SerialUSB.availableForWrite() < 2)
				return;
			SerialUSB.write(0xfd);
			c--;
		} else {
			// Normal
			if (SerialUSB.availableForWrite() < 1)
				return;
		}
		SerialUSB.write(c);
		ptr++;
		slider_tx_pending--;
	}
	slider_resp.cmd = 0;
}
