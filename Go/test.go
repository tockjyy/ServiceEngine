package main

import (
	"network/networkmgr"
)

//initial listener and run
func main() {
	//netmgr := networkmgr.NewNetWorkMgr()
	networkmgr.Instance().AddListen("test", "tcp", "0.0.0.0:7777")
	networkmgr.Instance().Runing()
	for {

	}
}
