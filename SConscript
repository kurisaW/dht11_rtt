from building import * 

# get current dir path
cwd = GetCurrentDir()

src = []
inc = [cwd]

src += ['rt_hw_dht11.c']

if GetDepend(['PKG_USING_DHT11_SAMPLE']):
    src += ['dht11_sample.c']

if GetDepend('PKG_DHT11_USING_SENSOR_V1'):
    src += ['sensor_dallas_dht11.c']

group = DefineGroup('dht11', src, depend = ['PKG_USING_DHT11'], CPPPATH = inc)
Return('group')
