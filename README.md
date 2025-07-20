# Cokkie-lang

> A minimal interpreted language written in C. Made for fun, chaos, and experiments.

📦 Prerequisites
What you need 

<br>
🀄curl
<br>
🐧linux
<br>
💻gcc our clang(while your architecture not is armv7)
<br>

## 🛠 Installation

Steps to install and run:

``` bash

curl https://github.com/voidh7/Cokkie-lang/blob/main/runCokkie_armv7
````
⚠️ This binary is built for **ARMv7 (32-bit)**.  
If you're on **x86_64**, **ARM64**, or another architecture, it won't work.  
👉 Please compile the source code manually for your system.

in gcc
``` bash
curl https://github.com/voidh7/Cokkie-lang/blob/main/SourceCode/runCokkie.c && gcc runCokkie.c -o runCokkie

```

in clang 
```bash
curl https://github.com/voidh7/Cokkie-lang/blob/main/SourceCode/runCokkie.c && clang runCokkie.c -o runCokkie
```

## 🧠 run your cokkie code 

Create a folder
``` bash
midir my-project-cokkie && cd my-project-cokkie
```

install cokkie a sdk 
``` bash
curl https://github.com/voidh7/Cokkie-lang/blob/main/runCokkie_armv7
```

create your main.cokkie
open in a text editor
run your code

cokkie code exenple
``` cokkie
function main(){
out.println(hello word)
}
```

run your code
``` bash
./runCokkie
```


