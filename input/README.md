# Sample Case Input Template

| Input	| Unit Symbol	| Unit name 		| Data Type	|
| :-----| :-------------| :--------------------:| :-------------|
| RATE 	| Mbps		| (megabits per second) | float		|
| DELAY	| ms		| (milliseconds) 	| float		|
| START	| s		| (seconds) 		| float		|
| DATA	| MB		| (megabytes) 		| float		|
| BUFFER| KB		| (kilobytes) 		| float		|
| END 	| s		| (seconds)		| float		|

``` 
// RapidJson supports comments
{
    "hosts": [ "H1", "H2", ... , "H<h>" ],
    "routers": [ "R1", "R2", ... , "R<r>" ],
    "links": [ 
        { "id": "L1", 
          "rate": <rate in Mbps>, 
          "delay": <signal propagation delay in ms>,
          "buffer": <buffer size in KB>,
          "endpoints": [ <host or router string>, ... , <host or router string> ] },
        ... ,
        { "id": "L<l>", 
          "rate": <rate in mbps>, 
          "delay": <signal propagation delay in ms>,
          "buffer": <buffer size in KB>,
          "endpoints": [ <host or router string>, ... , <host or router string> ] } ],
    "flows": [
        { "id": "F1",
          "src": <host string>,
          "dst": <host string>,
		  "protocol": <TCP Protocol string>,
          "size": <data transmission size in MB>,
          "start": <flow start time in sec> },
        ... ,
        { "id": "F<f>",
          "src": <host string>,
          "dst": <host string>,
		  "protocol": <TCP Protocol string>,
          "size": <data transmission size in MB>,
          "start": <flow start time in sec> } ],
	"end": <end simulation time in sec>
}
```
