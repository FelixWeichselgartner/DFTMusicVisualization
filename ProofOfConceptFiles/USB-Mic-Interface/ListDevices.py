import pyaudio

def ListDevices():
	devices = pyaudio.PyAudio()
	for i in range(devices.get_device_count()):
		print(devices.get_device_info_by_index(i).get('name'))

	print()
	print('What we need is index 2: ')
	print(devices.get_device_info_by_index(2).get('name'))

if __name__ == "__main__":
	ListDevices()
