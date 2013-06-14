# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

  # Vagrant box configuration
  config.vm.box = "precise64"
  config.vm.box_url = "http://files.vagrantup.com/precise64.box"

  # Bootstrap script
  config.vm.provision :shell, :path => "tools/vagrant/bootstrap.sh"

  # Forward SSH agent to host
  config.ssh.forward_agent = true
  
end
