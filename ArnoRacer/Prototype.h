//Input
void InitInput(void);
void Input(void);
int ChkIn( int n );
bool IsStartPB(void);

//Analog
void InitAdc(void);
void ad_read(void);
void Analog(void);

int GetValue(int n);
void GetValues(int data[]);

//Hwsetup
void Hwsetup(void);

//Motor
void InitMotor( int rate );
void MotorDrive(int dir,int left,int right);
void MotorStart(void);
void MotorStop(void);
void MotorControl(void);

//Fuzzy
void InitFuzzy(void);
int Fuzzy(int newVal);