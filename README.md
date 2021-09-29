# lcron
unix like cron bindings for lua

# build
* get ccronexpr.c/ccronexpr.h from https://github.com/staticlibs/ccronexpr.git
* make your project
* build your lcronc.so (or .dll)
* that's all

# sample
```lua
local lcron = require("lcron")

lcron.add("name1", "* * * * * *", function()
    print("hello cron!\n")
end)

lcron.add("name2", "* * * * * *", function()
    print("hello cron2!\n")
end)

lcron.delete("name2")

-- wait for 1 sec
local now = os.time()
while now == os.time() do
    -- spin
end

lcron.update()

lcron.delete("name1")
lcron.delete("name666") -- not exist

```

# license
no license, any way you want.
