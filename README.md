# AnalogReadAsync
 
*Asynchronous version of analogRead().*

Lets you do other processing while waiting for the analog read to complete. Can notify you when the read is complete by callback, or you can poll for when the read is complete. Also supports free-running mode, where multiple analog reads are performed until stopped.

## Usage

### Polling

Using polling allows you to do other operations while waiting for the analog read to complete. This works the same way as the traditional analogRead(), but with a way for you to do useful work while waiting.

```C++
analogReadAsync(A0);

while (!analogReadComplete()) {
  // Do stuff while waiting
}

uint16_t value = getAnalogReadValue();
```


### Callback

You can provide a callback that will be called when the read is complete. The callback will receive the read value, plus a pointer to any data you might want to pass to the callback.

```C++
void analogReadComplete(uint16_t value, void *data)
{
  Serial.println(value);

}

void setup() {
  analogReadAsync(A0, analogReadComplete);
}
```

### Free-Running Mode

You can also enable a "free-running" mode, where a new analog read starts as soon as the previous one finishes. Simply call `setAnalogReadFreeRunning(true)` to enable free-running. Free-running works with both a callback and polling.

Note that you will get one more analog read if you disable free running in the callback. This is because a new read operation has already started by the time the callback is called.
