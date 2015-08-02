/* *****************************************************************************
ALE Node.js wrapper 
Copyright (c) 2015-2015 Andrew Ivanov
MIT License
**************************************************************************** */

#include <iostream>
#include <ale_interface.hpp>

#include <node.h>
#include <v8.h>

#ifdef __USE_SDL
  #include <SDL.h>
#endif

using namespace std;
using namespace v8;

int play(std::string rom_file) {

    ALEInterface ale;

    // Get & Set the desired settings
    ale.setInt("random_seed", 123);
    //The default is already 0.25, this is just an example
    ale.setFloat("repeat_action_probability", 0.25);

#ifdef __USE_SDL
    ale.setBool("display_screen", true);
    ale.setBool("sound", true);
#endif

    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    ale.loadROM(rom_file);

    // Get the vector of legal actions
    ActionVect legal_actions = ale.getLegalActionSet();

    float totalReward = 0;

    // Play 10 episodes
    for (int episode=0; episode<10; episode++) {
        while (!ale.game_over()) {
            Action a = legal_actions[rand() % legal_actions.size()];
            // Apply the action and get the resulting reward
            float reward = ale.act(a);
            totalReward += reward;
        }
        cout << "Episode " << episode << " ended with score: " << totalReward << endl;
        ale.reset_game();
    }

    return totalReward;
}

void Play(const v8::FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong ROM file path")));
    return;
  }

  //string romFile = ;
  v8::String::Utf8Value param1(args[0]->ToString());
  std::string romFile = std::string(*param1);    

  int totalReward = play(romFile);

  args.GetReturnValue().Set(Integer::New(isolate, totalReward));
}

void Init(Handle<Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(exports, "play", Play);
}

NODE_MODULE(ale, Init)