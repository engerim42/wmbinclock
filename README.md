# wmbinclock

wmBinClock shows the actual system time as binary clock. You have to add up the "bits" to get the time. The clock has a 24 hour format.

In the default mode (vertical), the display is like this:

```
+ + + + + +  <- 8
+ + + + + +  <- 4
+ + + + + +  <- 2
+ + + + + +  <- 1
H H M M S S
```

For example:

```
+ + + + + +  <- 8
+ + * + + *  <- 4
* + + + * *  <- 2
+ * * + * *  <- 1
2 1 5 0 3 7
```

And in the horizontal mode, it's like this:

```
+ + + + + + <- Hour
+ + + + + + <- Minute
+ + + + + + <- Second
+ + + + + + <- Day
+ + + + + + <- Month
```

For example:

```
+ * + * + * <- 21
* * + + * + <- 50
* + + * + * <- 37
+ + * * + * <- 13
+ + + * * + <- 06
```

General Usage: ./wmBinClock OPTIONS

Colors:
```
  -i COLOR, --coloron=COLOR             Select COLOR as LED on color
  -o COLOR, --coloroff=COLOR            Select COLOR as LED off color
  -d COLOR, --datecoloron=COLOR         Select COLOR as LED on color
                                        for date display
```

  Where COLOR is one off these colors:
  gray neonred gold cyan pink blue green darkred none

Display mode
```
  -m MODE, --mode=MODE                  Select display mode
  Where MODE is:  "horizontal" or "vertical"

  -s {on/off}, --superfluous={on/off}   Display e.g. led for 32 in hours
  -display (host:screen)                Use an alternative XWindow display
```

The default is "wmBinClock -i neonred -o gray -d green -m vertical -s on -display 0:0"

Thanks to the frenchie Fleur M. for giving me the idea of this program ;)
