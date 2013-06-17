
Vagrant - Virtualized Development for pgRouting


1. Install

Just download and install a binary package from the [Vagrant website](http://docs.vagrantup.com/v2/installation/)

For example on Debian squeeze 32bit system:

```
mkdir work
cd work
wget -N http://files.vagrantup.com/packages/7e400d00a3c5a0fdf2809c8b5001a035415a607b/vagrant_1.2.2_i686.deb
sudo dpkg -i vagrant_1.2.2_i686.deb
sudo apt-get install virtualbox
vagrant box add precise32 http://files.vagrantup.com/precise32.box
cd /path/to/pgrouting
```

2. Run 

Start the virtual machine:

```
vagrant up
```

or if you are using precise32

```
BOX=precise32 vagrant up
```

Then login with (Using ssh agent forwarding):

```
vagrant ssh
```

Move to shared directory:

```
cd /vagrant
```


23 Build pgRouting

```
tools/vagrant/build.sh
```

Read more about Vagrant in their [official documentation](http://docs.vagrantup.com).
