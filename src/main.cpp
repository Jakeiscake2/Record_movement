#include "main.h"
#include <fstream>
using namespace std;
using namespace pros;

Controller controls(E_CONTROLLER_MASTER);

Motor indexer(17, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor intake_roller(9);

Motor flywheel1(19, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor flywheel2(20, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

Motor FrontLeft(1, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor BackLeft(2, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);

Motor FrontRight(3, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor BackRight(11, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
int button_chose = 4;

void btn_0() {
  lcd::set_text(7, "left button pressed");
  button_chose = 1;
}
void btn_1() {
  lcd::set_text(7, "middle button pressed");
  button_chose = 2;
}
void btn_2() {
  lcd::set_text(7, "hard roller selected");
  button_chose = 3;
}

void initialize() {
  indexer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  lcd::initialize();
  indexer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  lcd::set_text(1, "Screen working");
  controls.set_text(0, 0, "Controller Working");
}

void disabled() {}
void competition_initialize() {}
void autonomous() {
  indexer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  intake_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  int left_speeds = 0, right_speeds = 0;
  bool intake_on = false;
  int indexer_movements = 0;

  indexer.tare_position();
  indexer.move_absolute(169, 127);
  indexer.tare_position();

  ifstream movement_file;
  movement_file.open("/usd/hard_roller_movements.txt", ios::in);
  if (!movement_file.is_open()) {
    lcd::set_text(1, "File didnt open");
  } else {
    lcd::set_text(1, "Yipee file is open!!");
  }

  string inputed = "";
#include <vector>
  vector<string> vs;

  while (getline(movement_file, inputed)) {
    vs.push_back(inputed);
  }

  movement_file.close();
  lcd::set_text(1, "File should) be closed");
  int after_N;

  for (string line : vs) {
    left_speeds = stoi(line.substr(line.find('L') + 1, line.find('R') - 1));
    right_speeds = stoi(line.substr(line.find('R') + 1, line.find('N') - 1));

    left_speeds = left_speeds;
    right_speeds = right_speeds;
    FrontLeft = left_speeds;
    BackLeft = left_speeds;
    FrontRight = right_speeds;
    BackRight = right_speeds;

    after_N = line.find('N') + 1;
    if (line[after_N] == '1') {
      indexer_movements++;
    }

    indexer.move_absolute(indexer_movements * 360, 127);

    lcd::set_text(2, line);
    lcd::set_text(3, to_string(after_N));
    lcd::set_text(4, to_string(line[after_N + 1]));
    lcd::set_text(5, to_string(line[after_N] == 1));

    if (line[after_N + 1] == '1' && !intake_on) {
      intake_roller = 127;
      intake_on = !intake_on;
    } else if (line[after_N + 1] == '1' && intake_on) {
      intake_roller = 0;
      intake_on = !intake_on;
    }

    if (line[after_N + 2] == '1') {
      intake_roller = 69;
    } else if (line[after_N + 2] == '1' && !intake_on) {
      intake_roller = 0;
    }

    if (line[after_N + 3] == '1') {
      flywheel1 = 106;
      flywheel2 = 106;
    } else if (line[after_N + 4] == '1') {
      flywheel1 = 0;
      flywheel2 = 0;
    }

    lcd::set_text(4, to_string(indexer.get_actual_velocity()));
    controls.set_text(0, 0, line);
    delay(50);
  }
}
void opcontrol() {

  indexer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  indexer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  intake_roller.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  int x1, y1, x2, y2 = 0;
  int start_time = int(millis());
  int time_passed = int(millis()) - start_time;
  int intake_timer = int(millis()), indexer_timer = int(millis());
  int left_speeds = 0, right_speeds = 0;
  bool intake_on = false;
  int indexer_movements = 0;
  string time_message = "";
  int flywheel_speed = 0;
  bool indexer_brokey = true;

  string movement_s = "";
  vector<string> movement_v;
  delay(1000);

  int indexer_rotation = 0;
  int bindexer, bintake, bflywheel_on, bflywheel_off, broller;
  while (true) {
    bindexer = 0;
    bintake = 0;
    bflywheel_on = 0;
    bflywheel_off = 0;
    broller = 0;

    // x2 = controls.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
    // y2 = controls.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);

    // x1 = controls.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
    // y1 = controls.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
    // left_speeds = y2 - y1 - x1 - x2;
    // right_speeds = y2 - y1 + x1 + x2;
    // FrontLeft = left_speeds;
    // BackLeft = left_speeds;
    // FrontRight = right_speeds;
    // BackRight = right_speeds;
    // movement_s += "L" + to_string(left_speeds) + "R" +
    // to_string(right_speeds) + "N";

		int x, y;
    x = -(controls.get_analog(E_CONTROLLER_ANALOG_LEFT_X));
    y = -(controls.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));

    int maximum = max(abs(y), abs(x));
    int total = y + x;
    int difference = y - x;

    int xr, yr;
    xr = -(controls.get_analog(E_CONTROLLER_ANALOG_RIGHT_X));
    yr = -(controls.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));

    int maximumr = max(abs(yr), abs(xr));
    int totalr = yr + xr;
    int differencer = yr - xr;

    if (y == 0 && x == 0 && yr == 0 && xr == 0) {
      FrontLeft.brake();
      FrontRight.brake();
      BackLeft.brake();
      BackRight.brake();
    } else if (y >= 0) {
      if (x >= 0) {
        FrontLeft = maximum;
        BackLeft = maximum;
        FrontRight = difference;
        BackRight = difference;
      } else {
        FrontLeft = total;
        BackLeft = total;
        FrontRight = maximum;
        BackRight = maximum;
      }
    } else {
      if (x >= 0) {
        FrontLeft = total;
        BackLeft = total;
        FrontRight = -maximum;
        BackRight = -maximum;
      } else {
        FrontLeft = -maximum;
        BackLeft = -maximum;
        FrontRight = difference;
        BackRight = difference;
      }
    }
    if (yr == 0 && xr == 0 && y == 0 && x == 0) {
      FrontLeft.brake();
      FrontRight.brake();
      BackLeft.brake();
      BackRight.brake();
    } else if (yr >= 0) {
      if (xr >= 0) {
        FrontLeft = -maximum;
        BackLeft = -maximum;
        FrontRight = -difference;
        BackRight = -difference;
      } else {
        FrontLeft = -total;
        BackLeft = -total;
        FrontRight = -maximum;
        BackRight = -maximum;
      }
    } else {
      if (xr >= 0) {
        FrontLeft = -total;
        BackLeft = -total;
        FrontRight = maximum;
        BackRight = maximum;
      } else {
        FrontLeft = maximum;
        BackLeft = maximum;
        FrontRight = -difference;
        BackRight = -difference;
      }
    }
    
    indexer_rotation = int(indexer.get_position()) % 360;
    if (controls.get_digital(E_CONTROLLER_DIGITAL_R2) &&
        indexer_timer <= int(millis())) {
      indexer_movements++;
      indexer_timer = int(millis()) + 690;
    }

    indexer.move_absolute(169 + (indexer_movements * 360), 127);
    /*
    if(controls.get_digital(E_CONTROLLER_DIGITAL_B)){
            indexer_brokey = !indexer_brokey;
    }

    if (controls.get_digital(E_CONTROLLER_DIGITAL_R2) && indexer_timer <=
    int(millis())&&!indexer_brokey)
    {
            indexer = 127;
            indexer_timer = int(millis()) + 690;
            bindexer = 1;
    }
    else if (limit_switch.get_value() && indexer_timer <= int(millis()))
    {
            indexer = 7;
    }
    else if(!limit_switch.get_value() && indexer_timer <= int(millis()))
    {
            indexer = 127;
    }
            */
    lcd::set_text(2, to_string(indexer_rotation));
    lcd::set_text(3, to_string(indexer.get_actual_velocity()));

    if (controls.get_digital(E_CONTROLLER_DIGITAL_R1) &&
        intake_timer <= int(millis()) && !intake_on) {
      intake_roller = 127;
      intake_on = !intake_on;
      intake_timer = int(millis()) + 500;
      bintake = 1;
    } else if (controls.get_digital(E_CONTROLLER_DIGITAL_R1) &&
               intake_timer <= int(millis()) && intake_on) {
      intake_roller = 0;
      intake_on = !intake_on;
      intake_timer = int(millis()) + 500;
      bintake = 1;
    }

    if (controls.get_digital(E_CONTROLLER_DIGITAL_UP)) {
      intake_roller = 69;
      broller = 1;
    } else if (controls.get_digital(E_CONTROLLER_DIGITAL_UP) && !intake_on) {
      intake_roller = 0;
      broller = 1;
    }

    if (controls.get_digital(E_CONTROLLER_DIGITAL_L2)) {
      flywheel_speed = 96;
    }
    if (controls.get_digital(E_CONTROLLER_DIGITAL_L1)) {
      flywheel_speed = 0;
      bflywheel_off = 1;
    }

    if (controls.get_digital(E_CONTROLLER_DIGITAL_X)) {
      flywheel_speed = 106;
      bflywheel_on = 1;
    }

    if (controls.get_digital(E_CONTROLLER_DIGITAL_B)) {
      flywheel_speed = -69;
    }

    flywheel1 = flywheel_speed;
    flywheel2 = flywheel_speed;
    lcd::set_text(6, to_string(flywheel_speed));

    if (controls.get_digital(E_CONTROLLER_DIGITAL_Y)) {
      ofstream movement_add;
      movement_add.open("/usd/movements.txt", ios::out);

      if (!movement_add.is_open()) {
        lcd::set_text(1, "File didnt open");
      } else {
        lcd::set_text(1, "Yippe file is open!!");
      }
      for (string line : movement_v) {
        movement_add << line;
      }

      movement_add.close();
      lcd::set_text(1, "Congrats you have closed the file!");
      delay(60);
    }
    movement_s += to_string(bindexer) + to_string(bintake) +
                  to_string(broller) + to_string(bflywheel_on) +
                  to_string(bflywheel_off);
    movement_v.push_back(movement_s);

    time_passed = int(millis()) - start_time;
    time_message = "Time passed:" + to_string(time_passed / 1000) +
                   " Time left:" + to_string((105000 - time_passed) / 1000);
    lcd::set_text(4, to_string(indexer.get_actual_velocity()));
    controls.set_text(0, 0, time_message);
    delay(50);
  }
}

// 70 for small flex wheels
//  vince 50
//  yulian 20

// everyone pays 15
// vince gets 35 dollars
// yulian gets 5 dollars

// 35
// yulian 15
// vince 20

// everyone pays 6 dollars
// yulian gets 9 dollars
// vince gets 14 dollars

// everyone brings 21
