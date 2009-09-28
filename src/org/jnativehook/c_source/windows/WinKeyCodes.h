typedef struct {
	unsigned char keycode;
	bool shift_mask;
	bool control_mask;
	bool alt_mask;
	bool meta_mask;
} KeyCode;

typedef struct {
	unsigned char buttoncode;
	bool shift_mask;
	bool control_mask;
	bool alt_mask;
	bool meta_mask;
} ButtonCode;
