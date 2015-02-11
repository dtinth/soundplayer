soundplayer
===========

A very simple sampled sound file player written using FMOD Ex library, so can play WAV, MP3, OGG, and a lot more. Communicates via standard input.

So to compile, compile with [FMOD Ex](http://www.fmod.org/).


Building
--------

The provided Makefile assumes that you put your FMOD Ex installation at `/opt/fmodex`.


Usage
-----

This app receives commands from the standard input, and log messages to standard error.

Available commands are:

* `s` - stops all playing sound
* `lPATH` - loads the sample file at PATH but don't play it yet (won't load same file twice)
* `pPATH` - plays the loaded sample file at PATH (or load if it isn't already loaded)
* `vVOLUME` - sets the volume of the next sound to play (0.0 to 1.0, inclusive)

Example usage:

    l/home/dtinth/wav/wav01.wav
    p/home/dtinth/wav/wav01.wav
    p/home/dtinth/wav/wav02.wav
    s
    p/home/dtinth/wav/wav03.wav
    s


Why
---

So I can use it in other languages that requires playing sampled sound without fiddling to find a library that can play sounds well or
try to learn to write a wrapper around some library. For example, using this, I can quickly program my own sampler, as simple as:

    var stdin = process.openStdin();
    require('tty').setRawMode(true);

    function soundplayer() {
        var proc = require('child_process').spawn('/Users/dttvb/Projects/html5tnk/html5tnk/soundplayer');
        return {
            process: proc,
            load: function(file) { proc.stdin.write('l' + file + '\n'); },
            play: function(file) { proc.stdin.write('p' + file + '\n'); },
            quit: function(file) { proc.stdin.end(); }
        };
    }

    var p = soundplayer();
    p.process.stderr.pipe(process.stderr);
    p.process.stdout.pipe(process.stdout);
    p.load('kick.wav');
    p.load('hat.wav');
    p.load('clap.wav');

    process.stdin.on('keypress', function(chunk, key) {
        if (key.name == 'q') { p.quit(); process.exit(0); }
        if (key.name == 'd') p.play('kick.wav');
        if (key.name == 'f') p.play('clap.wav');
        if (key.name == 'j') p.play('clap.wav');
        if (key.name == 'k') p.play('hat.wav');
        console.log(key);
    });                 

