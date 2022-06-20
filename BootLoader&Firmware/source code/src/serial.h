#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>

enum {
	SLIDER_CMD_AUTO_SCAN = 0x01,
	SLIDER_CMD_SET_LED = 0x02,
	SLIDER_CMD_AUTO_SCAN_START = 0x03,
	SLIDER_CMD_AUTO_SCAN_STOP = 0x04,
	SLIDER_CMD_DIVA_UNK_09 = 0x09,
	SLIDER_CMD_DIVA_UNK_0A = 0x0A,
	SLIDER_CMD_RESET = 0x10,
	SLIDER_CMD_GET_BOARD_INFO = 0xF0
};

typedef union slider_packet {
	struct {
		uint8_t syn;
		uint8_t cmd;
		uint8_t size;
		union {
			struct {
				uint8_t led_unk;
				uint8_t leds[96];
			};
			char version[32];
			uint8_t pressure[32];
		};
	};
	uint8_t data[128];
} slider_packet_t;

/// Request and response data
extern slider_packet_t slider_req, slider_resp;
/// A message is being sent. Serial buffers should not be changed. FastLED or
/// any other library should not mask interrupts.
extern int slider_tx_pending;

// extern int checkcom;

/**
 *	Read slider request from serial
 *
 *	Reads from `Serial` and decodes the incoming request when a complete packet
 *	is correctly received. Writes the decoded request into `slider_req`. Call to
 *	this function will invalidate `slider_req` until a valid opcode is returned.
 *	@note This function is stateful! Carefully examine its implementation if you
 *	      wish to reuse it!
 *	@return The opcode, or 0 if a complete request is not yet received.
 */
uint8_t sliderserial_readreq();//读取数据

/**
 *	Send response to host
 *
 *	Encodes the response stored in `slider_resp` and send over `Serial`.
 *	`slider_resp` should not be modified until `slider_tx_pending` is zeroed.
 *	This function should be called on each loop iteration.
 *	@note This function is stateful! Carefully examine its implementation if you
 *	      wish to reuse it!
 */
void sliderserial_writeresp();//写入数据

#endif
