# -*- mode: ruby -*-
# vi: set ft=ruby :

pgbox    = ENV['BOX'] || "precise64"
postgres = ENV['POSTGRESQL_VERSION'] || "9.1"
postgis  = ENV['POSTGIS_VERSION']    || "2.0"

Vagrant.configure("2") do |config|

  # Vagrant box configuration
	config.vm.box = pgbox
	config.vm.box_url = "http://files.vagrantup.com/%s.box" % [pgbox]

  # Bootstrap script
  config.vm.provision :shell, :path => "tools/vagrant/bootstrap.sh", :args => "%s %s" % [postgres, postgis]

  # Forward SSH agent to host
  config.ssh.forward_agent = true
  
  # Create synced folder for GnuPG keys and within home directory
  config.vm.synced_folder "../", "/home/vagrant/repos"
  config.vm.synced_folder "~/.gnupg", "/home/vagrant/.gnupg"

end
