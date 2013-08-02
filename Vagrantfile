# -*- mode: ruby -*-
# vi: set ft=ruby :

pgbox    = ENV['BOX'] || "precise64"
postgres = ENV['POSTGRESQL_VERSION'] || "9.1"
postgis  = ENV['POSTGIS_VERSION']    || "2.0"

Vagrant.configure("2") do |config|

  # Vagrant box configuration
  case pgbox
  when "pgr32"
    config.vm.box = "pgr32"
    config.vm.box_url = "http://s3.amazonaws.com/pgrouting/pgrouting-precise32.box"

  when "pgr64"
    config.vm.box = "pgr64"
    config.vm.box_url = "http://s3.amazonaws.com/pgrouting/pgrouting-precise64.box"

  when "precise32", "precise64"
  	config.vm.box = pgbox
  	config.vm.box_url = "http://files.vagrantup.com/%s.box" % [pgbox]
    
  end

  # Bootstrap script
  config.vm.provision :shell, :path => "tools/vagrant/bootstrap.sh", :args => "%s %s" % [postgres, postgis]

  # Forward SSH agent to host
  config.ssh.forward_agent = true
  
  # Create synced folder for GnuPG keys and within home directory
  config.vm.synced_folder "../", "/home/vagrant/repos"
  config.vm.synced_folder "~/.gnupg", "/home/vagrant/.gnupg"
  config.vm.synced_folder "~/.transifexrc", "/home/vagrant/.transifexrc"

end
