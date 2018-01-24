# What's new?

## Version 2.2 

Teletype version 2.2 introduces Chaos and Bitwise operators, Live mode view of variables, INIT operator, ability to calibrate CV In and Param knob and set Min/Max scale values for both, a screensaver, Random Number Generator, and a number of fixes and improvements.

### Major new features

#### Chaos Operators

The `CHAOS` operator provides a new source of uncertainty to the Teletype via chaotic yet deterministic systems. This operator relies on various chaotic maps for the creation of randomized musical events. Chaotic maps are conducive to creating music because fractals contain a symmetry of repetition that diverges just enough to create beautiful visual structures that at times also apply to audio. In mathematics a map is considered an evolution function that uses polynomials to drive iterative procedures. The output from these functions can be assigned to control voltages. This works because chaotic maps tend to repeat with slight variations offering useful oscillations between uncertainty and predictability.

#### Bitwise Operators

Bitwise operators have been added to compliment the logic functions and offer the ability to maximize the use of variables available on the Teletype.

Typically, when a variable is assigned a value it fully occupies that variable space; should you want to set another you’ll have to use the next available variable. In conditions where a state of on, off, or a bitwise mathematical operation can provide the data required, the inclusion of these operators give users far more choices. Each variable normally contains 16 bits and Bitwise allows you to `BSET`, `BGET`, and `BCLR` a value from a particular bit location among its 16 positions, thus supplying 16 potential flags in the same variable space. 

#### INIT

The new op family `INIT` features operator syntax for clearing various states from the unforgiving INIT with no parameters that clears ALL state data (be careful as there is no undo) to the ability to clear CV, variable data, patterns, scenes, scripts, time, ranges, and triggers.

#### Live Mode Variable Display

This helps the user to quickly check and monitor variables across the Teletype. Instead of single command line parameter checks the user is now able to simply press the `~ key` (Tilde) and have a persistent display of eight system variables.

#### Screensaver

Screen saver engages after 90 minutes of inactivity

#### New Operators

   • `IN.SCALE min max` sets the min/max values of the CV Input jack
   • `PARAM.SCALE min max` set the min/max scale of the Parameter Knob
   • `IN.CAL.MIN` sets the zero point when calibrating the CV Input jack
   • `IN.CAL.MAX` sets the max point (16383) when calibrating the CV Input jack
   • `PARAM.CAL.MIN` sets the zero point when calibrating the Parameter Kob
   • `PARAM.CAL.MAX` sets the max point (16383) when calibrating the Parameter Kob
   • `R` generate a random number
   • `R.MIN` set the low end of the random number generator
   • `R.MAX` set the upper end of the random number generator

#### Fixes

   • Multiply now saturates at limits (-32768 / 32767) while previous behavior returned 0 at overflow
   • Entered values now saturate at Int16 limits which are -32768 / 32767
   • Reduced flash memory consumption by not storing TEMP script
   • I now carries across `DEL` commands
   • Corrected functionality of `JI` (Just Intonation) op for 1V/Oct tuning
   • Reduced latency of `IN` op

#### Improvements

   • Profiling code (optional developer feature)
   • Screen now redraws only lines that have changed

## Version 2.1

Teletype version 2.1 introduces new operators that mature the syntax and capability of the Teletype, as well as several bug fixes and enhancement features.

### Major new features

#### Tracker Data Entry Improvements

Data entry in the tracker screen is now _buffered_, requiring an `ENTER` keystroke to commit changes, or `SHIFT-ENTER` to insert the value.  All other navigation keystrokes will abandon data entry.  The increment / decrement keystrokes (`]` and `[`), as well as the negate keystroke (`-`) function immediately if not in data entry mode, but modify the currently buffered value in edit mode (again, requiring a commit).

#### Turtle Operator

The Turtle operator allows 2-dimensional access to the patterns as portrayed out in Tracker mode.  It uses new operators with the `@` prefix.  You can `@MOVE X Y` the turtle relative to its current position, or set its direction in degrees with `@DIR` and its speed with `@SPEED` and then execute a `@STEP`.

To access the value that the turtle operator points to, use `@`, which can also set the value with an argument.

The turtle can be constrained on the tracker grid by setting its fence with `@FX1`, `@FY1`, `@FX2`, and `@FY2`, or by using the shortcut operator `@F x1 y1 x2 y2`.  When the turtle reaches the fence, its behaviour is governed by its _fence mode_, where the turtle can simply stop (`@BUMP`), wrap around to the other edge (`@WRAP`), or bounce off the fence and change direction (`@BOUNCE`).  Each of these can be set to `1` to enable that mode.

Setting `@SCRIPT N` will cause script `N` to execute whenever the turtle crosses the boundary to another cell.  This is different from simply calling `@STEP; @SCRIPT N` because the turtle is not guaranteed to change cells on every step if it is moving slowly enough.

Finally, the turtle can be displayed on the tracker screen with `@SHOW 1`, where it will indicate the current cell by pointing to it from the right side with the `<` symbol.

#### New Mods: EVERY, SKIP, and OTHER, plus SYNC

These mods allow rhythmic division of control flow. EVERY X: executes the post-command once per X at the Xth time the script is called. SKIP X: executes it every time but the Xth. OTHER: will execute when the previous EVERY/SKIP command did not.

Finally, SYNC X will set each EVERY and SKIP counter to X without modifying its divisor value. Using a negative number will set it to that number of steps before the step. Using SYNC -1 will cause each EVERY to execute on its next call, and each SKIP will not execute.

#### Script Line "Commenting"

Individual lines in scripts can now be disabled from execution by highlighting the line and pressing `ALT-/`.  Disabled lines will appear dim.  This status will persist through save/load from flash, but will not carry over to scenes saved to USB drive.

### New Operators

`W [condition]:` is a new mod that operates as a while loop.
The `BREAK` operator stops executing the current script
`BPM [bpm]` returns the number of milliseconds per beat in a given BPM, great for setting `M`.
`LAST [script]` returns the number of milliseconds since `script` was last called.

### New Operator Behaviour

`SCRIPT` with no argument now returns the current script number.
`I` is now local to its corresponding `L` statement.
`IF/ELSE` is now local to its script.

### New keybindings

`CTRL-1` through `CTRL-8` toggle the mute status for scripts 1 to 8 respectively.
`CTRL-9` toggles the METRO script.
`SHIFT-ENTER` now inserts a line in Scene Write mode.

### Bug fixes

Temporal recursion now possible by fixing delay allocation issue, e.g.: DEL 250: SCRIPT SCRIPT
`KILL` now clears `TR` outputs and stops METRO.
`SCENE` will no longer execute from the INIT script on initial scene load.
`AVG` and `Q.AVG` now round up from offsets of 0.5 and greater.

### Breaking Changes

As `I` is now local to `L` loops, it is no longer usable across scripts or as a general-purpose variable.
As `IF/ELSE` is now local to a script, scenes that relied on IF in one script and ELSE in another will be functionally broken.

## Version 2.0

Teletype version 2.0 represents a large rewrite of the Teletype code base. There are many new language additions, some small breaking changes and a lot of under the hood enhancements.


### Major new features

#### Sub commands

Several commands on one line, separated by semicolons.

e.g. `CV 1 N 60; TR.PULSE 1` 

See the section on "Sub commands" for more information.
    
#### Aliases
  
For example, use `TR.P 1` instead of `TR.PULSE 1`, and use `+ 1 1`, instead of `ADD 1 1`.

See the section on "Aliases" for more information.

#### `PN` versions of every `P` `OP`

There are now `PN` versions of every `P` `OP`. For example, instead of:

```
P.I 0
P.START 0
P.I 1
P.START 10
```

You can use:

```
PN.START 0 0
PN.START 1 10
```

#### TELEXi and TELEXo `OP`s

Lots of `OP`s have been added for interacting with the wonderful TELEXi input expander and TELEXo output expander. See their respective sections in the documentation for more information.

#### New keybindings

The function keys can now directly trigger a script.

The `<tab>` key is now used to cycle between live, edit and pattern modes, and there are now easy access keys to directly jump to a mode.

Many new text editing keyboard shortcuts have been added.

See the "Modes" documentation for a listing of all the keybindings.

#### USB memory stick support

You can now save you scenes to USB memory stick at any time, and not just at boot up. Just insert a USB memory stick to start the save and load process. Your edit scene should not be effected.

It should also be significantly more reliable with a wider ranger of memory sticks.

**WARNING:** Please backup the contents of your USB stick before inserting it. Particularly with a freshly flashed Teletype as you will end up overwriting all the saved scenes with blank ones.

### Other additions

 - Limited script recursion now allowed (max recursion depth is 8) including self recursion.
 - Metro scripts limited to 25ms, but new `M!` op to set it as low as 2ms (at your own risk), see "Metronome" `OP` section for more.

### Breaking changes

  - **Removed the need for the `II` `OP`.**

    For example, `II MP.PRESET 1` will become just `MP.PRESET 1`.
 
  - **Merge `MUTE` and `UNMUTE` `OP`s to `MUTE x` / `MUTE x y`.**
  
    See the documentation for `MUTE` for more information.

  - **Remove unused Meadowphysics `OP`s.**

    Removed: `MP.SYNC`, `MP.MUTE`, `MP.UNMUTE`, `MP.FREEZE`, `MP.UNFREEZE`.

  - **Rename Ansible Meadowphysics `OP`s to start with `ME`.**

    This was done to avoid conflicts with the Meadowphysics `OP`s.
   
 **WARNING**: If you restore your scripts from a USB memory stick, please manually fix any changes first. Alternatively, incorrect commands (due to the above changes) will be skipped when imported, please re-add them.

### Known issues

#### Visual glitches

The cause of these is well understood, and they are essentially harmless. Changing modes with the `<tab>` key will force the screen to redraw. A fix is coming in version 2.1.
