#define USART_BAUDRATE 31250
#define BAUD_PRESCALE 39

#define MIDI_STATUS_TYPE_NOTEON 0x90
#define MIDI_STATUS_TYPE_NOTEOFF 0x80
#define MIDI_STATUS_TYPE_CC 0xB0
#define MIDI_STATUS_TYPE_PB 0xE0

#define MIDI_STATUS_NONE 0
#define	MIDI_STATUS_NOTEON 1
#define MIDI_STATUS_NOTEOFF 2
#define MIDI_STATUS_CC 3
#define MIDI_STATUS_PB 4

//Maximum and minimum timer values for aesthetic purposes
#define MAXIMUM_FREQ 10000
#define MINIMUM_FREQ 25

unsigned int shift_register;
unsigned char clock_ready; //Variable to store when timer overflow occurred, move clock function out of interrupt
unsigned char byte_received;
unsigned char playing_midi_note;
unsigned char current_midi_status;
unsigned char current_midi_channel;
unsigned char current_midi_note;
unsigned char current_midi_velocity;
unsigned char current_midi_cc;
unsigned char current_midi_ccdata;
unsigned char current_midi_pb_l;
unsigned char current_midi_pb_h;
unsigned int current_midi_pb;
unsigned int fine_pitch_bend;
unsigned char midi_channel;

#define SWEEP_UP 1
#define SWEEP_DOWN 0

enum cc_mesages {
	NOISE_TYPE_CC = 1,
	MASTER_VOLUME_CC = 7,
	FINE_PITCH_CC = 8,
	VOLDECAY_ENABLED_CC,
	VOLDECAY_AMOUNT_CC,
	VOLDECAY_LOOP_ENABLED_CC,
	SWEEP_ENABLED_CC,
	SWEEP_DIRECTION_CC,
	SWEEP_AMOUNT_CC,
	SWEEP_LOOP_ENABLED_CC
};

unsigned char num_bytes;
unsigned char num_ccs;
unsigned char num_pbs;

unsigned char noise_type;
unsigned char tap_one; //Input 1 of xor operation
unsigned char tap_two; //Input 2 of xor operation
unsigned int frequency;
unsigned char amplitude;

unsigned char fake_8_timer; //to slow down decay/pitch

unsigned char master_volume;

unsigned char voldecay_enabled;
unsigned char voldecay_amount;
unsigned char voldecay_loop_enabled;

unsigned char sweep_enabled;
unsigned char sweep_direction;
unsigned char sweep_amount;
unsigned char sweep_loop_enabled;

unsigned int distance;

unsigned char byte_ready;

void init_interrupts(void);
void enable_USART_interrupts(void);
void disable_USART_interrupts(void);
void init_io(void);
void note_on(void);
void note_off(void);
void process_cc(void);
void bend_pitch(void);
void update_frequency(unsigned int new_frequency);

void clear_byte_received(void);
void check_byte_received(void);
void check_channel_set(void); //A 1 on PD3 will use Channels 0-4, 0 will make it use Channels 5-9

void note_on(void);
void note_off(void);
void init_interrupts(void);
void init_io(void);
void init_timers(void);

void clock_shift_register(void);

//Timer values for musical notes
//For Prescaler = 1024
const unsigned int note_table[128] = { 19111, 18039, 17026, 16071, 15169,
		14317, 13514, 12755, 12039, 11364, 10726, 10124, 9556, 9019, 8513,
		8035, 7584, 7159, 6757, 6378, 6020, 5682, 5363, 5062, 4778, 4510, 4257,
		4018, 3792, 3579, 3378, 3189, 3010, 2841, 2681, 2531, 2389, 2255, 2128,
		2009, 1896, 1790, 1689, 1594, 1505, 1420, 1341, 1265, 1194, 1127, 1064,
		1004, 948, 895, 845, 797, 752, 710, 670, 633, 597, 564, 532, 502, 474,
		447, 422, 399, 376, 355, 335, 316, 299, 282, 266, 251, 237, 224, 211,
		199, 188, 178, 168, 158, 149, 141, 133, 126, 119, 112, 106, 100, 94,
		89, 84, 79, 75, 70, 67, 63, 59, 56, 53, 50, 47, 44, 42, 40, 37, 35, 33,
		31, 30, 28, 26, 25, 24, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12 };
