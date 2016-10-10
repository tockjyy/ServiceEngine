package networkmgr

import "network/sessionpool"
import "proto/protoqueue"
import "proto/protohandle"

var _instance *NetWorkMgr

func Instance() *NetWorkMgr {
	if _instance == nil {
		_instance = NewNetWorkMgr()
	}
	return _instance
}

type NetWorkMgr struct {
	listenqueue map[string]sessionpool.SessionPool
	//connectqueue chan asyncconnect.AsyncConnect
}

func (netmgr *NetWorkMgr) WriteBuf(name string, conid int, buf []byte) {
	listen, ret := netmgr.listenqueue[name]
	if !ret {
		return
	}
	listen.WriteBuf(conid, buf)
}

func (netmgr *NetWorkMgr) AddListen(lname string, ltype string, ipport string) {
	spool := sessionpool.NewSessionPool(lname, ltype, ipport)
	netmgr.listenqueue[lname] = spool
}

func NewNetWorkMgr() *NetWorkMgr {
	t := new(NetWorkMgr)
	t.listenqueue = map[string]sessionpool.SessionPool{}
	return t
}

func (netmgr *NetWorkMgr) Runing() {
	for _, spool := range netmgr.listenqueue {
		go func(pool sessionpool.SessionPool) {
			pool.Runing()
		}(spool)
	}
	go func() {
		for task := range protoqueue.Instance().GetSendQueue() {
			netmgr.WriteBuf(task.Name(), task.Conid(), task.Buf())
		}
	}()
	go func() {
		for task := range protoqueue.Instance().GetRecvQueue() {
			protohandle.HandleProto(task)
		}
	}()
}
