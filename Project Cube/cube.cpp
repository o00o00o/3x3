#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
#include <vector>
#include <windows.h>


class times {
    struct split {
      //linked list to store data
      std::chrono::microseconds elapsed;
      split *next;
    };
    split *head;
    split best;
    int num_split;
  public:
    //int num_split;
    times();
    void new_split(std::chrono::microseconds add);
    void display(split);
    void display_all();
    void display_latest();
    void display_best();
    void clear();
    void avg();
    void avg5();
    void avg10();

};

times::times() {
  //initialize head and num_split
  head = 0;
  num_split = 0;
}

void times::new_split(std::chrono::microseconds add) {
  //always push onto head of the list
  if (num_split == 0) {
    //if list is empty
    split *temp = new split;
    temp->elapsed = add;
    head = temp;
    temp->next = 0;
    num_split++;
    //assign best split
    best.elapsed = add;
    best.next = 0;
  }
  else {
      split *temp = new split;
      temp->elapsed = add;
      temp->next = head;
      head = temp;
      num_split++;
      //check for best time
      if (add < best.elapsed) {
        best.elapsed = add;
      }
  }
}

void times::display(split x) {
  using namespace std::chrono;
  //cout time in format "min:sec:milli:micro"
  if (num_split == 0) {
    std::cerr << "No splits recorded" << std::endl;
    return;
  }

  //conversion from duration
  unsigned long long int total = x.elapsed.count();
  unsigned min = 0;
  unsigned sec = 0;
  unsigned milli = 0;
  unsigned micro = 0;

  min = total/60000000;
  total = total%60000000;

  sec = total/1000000;
  total = total%1000000;

  milli = total/1000;
  total = total%1000;

  micro = total;

  std::cout << min << ":" << sec << ":" << milli << ":" << micro << std::flush;
  return;
}

void times::display_best() {
  //displays best time
  display(this->best); return;
}

void times::display_latest() {
  //displays latest time
  display(*head); return;
}

void times::display_all() {
  //displays all recorded time, starting from most recent
  if (num_split == 0) {
    //error when list empty
    std::cerr << "No splits recorded." << std::endl;
  }

  int j = num_split;             //numbers each displayed time
  split *i = head;               //starting from latest (head)
  while (i) {
    std::cout << j << ".  ";
    display(*i);                 // using display() to print
    std::cout << std::endl;
    i = i->next;
    j--;
  }
  return;
}

void times::clear() {
  //clear linked list, reset num_split, reset head;
  split *i = head;
  while (i != 0) {
    split *j = i->next;
    delete i;
    i = j;
    num_split--;
  }
  head = 0;
}

void times::avg() {
  //finds and display avg of all recorded time
  if (num_split == 1) {
    display_latest();
    return;
  }

  split *i = head;
  split temp;
  temp.elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::duration::zero());
  while (i) {
    temp.elapsed = temp.elapsed + i->elapsed;  //sum up total time
    i = i->next;
  }
  temp.elapsed = temp.elapsed/num_split;  //divided by number of splits
  display(temp);
}

void times::avg5() {
  //find avg of top 5 time
  using namespace std::chrono;
  if (num_split < 5) {
    std::cerr << "Less than 5 recorded splits.";
    return;
  }

  //create array with all recorded time
  microseconds a_sort[num_split];
  split *i = head;
  for (int j = 0; j < num_split; j++) {
    a_sort[j] = i->elapsed;
    i = i->next;
  }

  //using vectors, partial sort for top 5 spllit
  std::vector<microseconds> v_sort(a_sort, a_sort + num_split);
  std::partial_sort (v_sort.begin(), v_sort.begin()+5, v_sort.end());

  //total time for top 5 split
  microseconds top5;
  for (int i = 0; i < 5; i++) {
    top5 += v_sort[i];
  }

  //find avg of top 5
  split avg_of_5;
  avg_of_5.elapsed = top5/5;
  display (avg_of_5);
  return;
}

  void times::avg10() {
    //find avg of top 10 time
    using namespace std::chrono;
    if (num_split < 10) {
      std::cerr << "Less than 10 recorded splits." << std::endl;
      return;
    }

    //create array with all recorded time
    microseconds a_sort[num_split];
    split *i = head;
    for (int j = 0; j < num_split; j++) {
      a_sort[j] = i->elapsed;
      i = i->next;
    }

    //using vectors, partial sort for top 10 spllit
    std::vector<microseconds> v_sort(a_sort, a_sort + num_split);
    std::partial_sort (v_sort.begin(), v_sort.begin() + 10, v_sort.end());

    //total time for top 10 split
    microseconds top10;
    for (int i = 0; i < 10; i++) {
      top10 += v_sort[i];
    }

  //find avg of top 10, return as split
  split avg_of_10;
  avg_of_10.elapsed = top10/10;
  display(avg_of_10);
  return;
}


class stop_watch {
    std::chrono::high_resolution_clock::time_point start_time; //time point start
    std::chrono::high_resolution_clock::time_point stop_time; //time point stop
  public:
    stop_watch(); //constuctor to initialize stop & stop to time_point::min
    void start();
    void stop();
    std::chrono::microseconds get_time(); //returns duration in microseconds
};

stop_watch::stop_watch() {
  //initialize time_point to min
  start_time = std::chrono::high_resolution_clock::time_point::min();
  stop_time = std::chrono::high_resolution_clock::time_point::min();
}

void stop_watch::start() {
  //start stop watch, initialize time_point start
  start_time = std::chrono::high_resolution_clock::now();
}

void stop_watch::stop() {
  //stops stop watch, initialize time_point stop
  stop_time = std::chrono::high_resolution_clock::now();
}

std::chrono::microseconds stop_watch::get_time() {
  //find duration between stop & start in microseconds
  using namespace std::chrono;
  return duration_cast<microseconds>(stop_time - start_time);
}


class rubik {
    std::string notation[18] = {"U ","L ","F ","R ", "B ", "D ",
                         " U' ", "L' ", "F' ", "R' ", "B' ", "D' ",
                         "U2 ","L2 ","F2 ","R2 ","B2 ", "D2 "};
    std::mt19937 engine;  //generate mt19937 engine
  public:
    rubik();
    int r();
    void scramble();

};

rubik::rubik() {
  //generate random device, seed mt19937 with random device
  std::random_device seeder;
  engine.seed(seeder());
}

int rubik::r() {
  //run MT engine and return random number
  std::uniform_int_distribution<int> distribution(0,17);
  return distribution(engine);
}

void rubik::scramble() {
  //display 25 random rotations
  //display in groups of 5
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      std::cout << notation[r()] << std::flush;
    }
    if (i != 4) {
      std::cout << "| " << std::flush;
    }
  }
}

void key_start() {
  //start button using left & right shift key
  bool x = false;
  while (true) {
  if (GetAsyncKeyState(VK_LSHIFT) < 0 && GetAsyncKeyState(VK_RSHIFT) < 0) {
    while (true) {
      if (GetAsyncKeyState(VK_LSHIFT) >= 0 && GetAsyncKeyState(VK_RSHIFT) >= 0) {
        x = true;
        break;
      }
    }
  }
  if (x == true) {
    return;
  }
  }
}

void key_stop() {
  //stop button using space bar
  while (true) {
    if (GetAsyncKeyState(VK_SPACE) < 0) {
      return;
    }
  }
}

void display_menu1() {
  using namespace std;
  cout << " 'Left Shift' AND 'Right Shift' -  Timer start" << endl
       << " 'Space'                        -  Timer stop" << endl;
}

void display_menu2() {
  using namespace std;
  cout << " 'Space' - Reset & generate new scramble" << endl
       << " 'A'     - Show all recorded times" << endl
       << " 'C'     - Clear all recorded times" << endl
       << " 'ESC'   - Exit program" << endl;
       //<< " 'S' - Save time as txt" << endl;     //implement this feature later, prompt user for file name
}


int options() {
  //using keystate to select options
  while (true) {
    if (GetAsyncKeyState(0x43) < 0)  return 1;  // 'C'
    else if(GetAsyncKeyState(VK_ESCAPE) < 0)  return 2;  // 'ESC'
    else if  (GetAsyncKeyState(VK_SPACE) < 0) return 3; // spacebar;
    else if  (GetAsyncKeyState(0x41) < 0) return 4; // 'A';
  }
}

void clear_console() {
  //clears clear_console
  for (int i = 0; i < 100; i++) {
    std::cout << std::endl;
  }
}

int main() {
  using namespace std;
  //class decleartions
  stop_watch timer;
  times my_time;
  rubik cube;

  while (true) {
    clear_console();
    display_menu1(); cout << endl;
    cube.scramble(); cout << endl;   //display scramble

    //timer
    key_start();
    timer.start();
    key_stop();
    timer.stop();

    my_time.new_split(timer.get_time());   //save time, add to linked list

    cout << endl
         << "Latest:        "; my_time.display_latest(); cout << endl << endl;
    cout << "Best:          "; my_time.display_best(); cout << endl;
    cout << "Average:       "; my_time.avg(); cout << endl;
    cout << "Average of 5:  "; my_time.avg5(); cout << endl;
    cout << "Average of 10: "; my_time.avg10(); cout << endl;

    display_menu2(); cout << endl;
    Sleep(400);
    int input = options();

    if (input != 3) {
      while (true) {
        //clear linked list
        if (input == 1) {
          my_time.clear();
          cout << endl << "Cleared!" << endl;

          Sleep(500);
          clear_console();
          display_menu2();
          Sleep(400);
          input = options();
          if (input == 3) break;
        }

        if (input == 4) {
          //display all recorded time
          clear_console();
          my_time.display_all(); cout << endl;

          display_menu2(); cout << endl;
          Sleep(400);
          input = options();
          if (input == 3) break;
        }

        if (input == 2) {
          //exit program
               return 0;
        }
      }
    }
  }
}
