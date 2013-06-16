# -*- mode: ruby -*-
# vi: set ft=ruby :

postgres = ENV['POSTGRESQL_VERSION'] || "9.1"
postgis  = ENV['POSTGIS_VERSION']    || "2.0"

Vagrant.configure("2") do |config|

  # Vagrant box configuration
  #config.vm.box = "precise64"
  #config.vm.box_url = "http://files.vagrantup.com/precise64.box"

  config.vm.box = "pgr64"
  config.vm.box_url = "http://s3.amazonaws.com/pgrouting/pgrouting-precise64.box"

  # Bootstrap script
  config.vm.provision :shell, :path => "tools/vagrant/bootstrap.sh", :args => "%s %s" % [postgres, postgis]

  # Forward SSH agent to host
  config.ssh.forward_agent = true
  
end
