#  Quick start

Clone this repository in your CloudGate SDK /package folder *(f.ex. CloudGate-SDK-x86_64-V2.82.0/package)*.

Run **make menuconfig** in CloudGate SDK root directory and include the cloned project in *"Base system --->"* menu.

## Enable ssh on CloudGate (optional)

Copy your public ssh key (~/.ssh/id_rsa.pub) in to **/files/ssh/authorized_keys** to enable ssh login to CloudGate.

Set up a password for root login & web admin panel by filling **/files/etc/passwd**. 

## Set Alibaba API keys

Set Alibaba Cloud API credentials in **src/config.h** by setting **PRODUCT_KEY, DEVICE_NAME** and **DEVICE_SECRET** to values corresponding to your Alibaba IoT Platform Device.

## Compile and upload the project

Run **make** in CloudGate SDK root directory to compile the project.

Upload the compiled package *(/bin/imx28/bundle-sdk-xxxxxxxxxxxxxx.zip)* to the CloudGate via CloudGate web interface *(accessible at 192.168.1.1/index.html/provisioning)* and reboot the CloudGate.

**Note:** Don't forget to disable automatic provisioning in the same menu to avoid the CloudGate rolling back to the default image.

**Note:** By default, the compiled application will execute on startup. This can be changed in the **isv** script.
