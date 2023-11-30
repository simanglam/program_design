#include <stdio.h>
#include <stdlib.h>

#include "include/raylib.h"
#include "include/audio.h"

static Music BgMusic;
static Music BoosterMusic;
static Music * CurrentMusic;

void MyAudioInit(){
    InitAudioDevice();
    BgMusic = LoadMusicStream("./music/pac_man_background.wav");
    BoosterMusic = LoadMusicStream("./music/pac_man_booster.wav");
    PlayBg();
}

void AudioUpdate(){
    UpdateMusicStream(*CurrentMusic);
}

void PlayBg(){
    CurrentMusic = &BgMusic;
    PlayMusicStream(*CurrentMusic);
}

void PlayBooster(){
    CurrentMusic = &BoosterMusic;
    PlayMusicStream(*CurrentMusic);
}

void SetVolume(float volume){
    SetMusicVolume(BgMusic, volume);
    SetMusicVolume(BoosterMusic, volume);
}
