# Vagrant - Virtualized Development for pgRouting

# Install

Just download and install a binary package from the [Vagrant website](http://docs.vagrantup.com/v2/installation/)

# Run 

Start the virtual machine:

```
vagrant up
```

Then login with (Using ssh agent forwarding):

```
vagrant ssh
```

Move to shared directory:

```
cd /vagrant
```

# Build pgRouting

```
tools/vagrant/build.sh
```

Read more about Vagrant in their [official documentation](http://docs.vagrantup.com).