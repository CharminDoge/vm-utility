# vm-utility
Toolkit fow VM startup and connection

The toolkit does:

  checks if host machine is ON, if not sends magick packet and tries to ping until it receives a respone
  
  // work in progress
  sends "M" message to host machine
  the host is listening for the "M" message and when it is received, an executable is run
  the executable starts the VM and sends response back
  the toolkit connects to the VM using mstsc.exe
