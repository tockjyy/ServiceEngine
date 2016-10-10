package sessionpool

import (
	"fmt"
	"net"
	"network/asyncsession"
	"os"
	"sync"
)

type SessionPool struct {
	ipool   map[int]*asyncsession.AsyncSession
	bpool   map[int]*asyncsession.AsyncSession
	count   int
	mutex   sync.Mutex
	rwmutex sync.RWMutex
	ipport  string
	ltype   string
	lname   string
}

func NewSessionPool(lname string, ltype string, ipport string) SessionPool {
	var t SessionPool
	t.ipool = map[int]*asyncsession.AsyncSession{}
	t.bpool = map[int]*asyncsession.AsyncSession{}
	t.count = 0
	t.ltype = ltype
	t.ipport = ipport
	t.lname = lname
	return t
}

func (pool *SessionPool) WriteBuf(conid int, buf []byte) {
	pool.rwmutex.RLock()
	defer pool.rwmutex.RUnlock()

	session, ok := pool.bpool[conid]
	if ok {
		session.WriteBuffer(buf)
		println("ok")
	}
}

func (pool *SessionPool) Malloc(num int) {
	//pool.rwmutex.RLock()
	//defer pool.rwmutex.RUnlock()
	for ; 0 != num; num-- {
		pool.count++
		id := pool.count
		session := new(asyncsession.AsyncSession)
		session.SetConnectIdAndName(int(id), pool.lname)
		pool.ipool[id] = session
	}
	fmt.Println(len(pool.ipool) + len(pool.bpool))
}

func (pool *SessionPool) Runing() {
	listener, err := net.Listen(pool.ltype, pool.ipport)
	if err != nil {
		fmt.Println("error listening:", err.Error())
		os.Exit(1)
	}
	defer listener.Close()

	for {
		conn, err := listener.Accept()
		if err != nil {
			println("Error accept:", err.Error())
			return
		}
		println("hava new connect")
		go func(conn net.Conn) {
			println("connect is on")
			session, _ := pool.PopSession()
			session.RecvHandle(conn)
			pool.PushSession(session.GetConnectId())
			println("connect is off")
		}(conn)
	}

}

func (pool *SessionPool) ISize() int {
	pool.rwmutex.RLock()
	defer pool.rwmutex.RUnlock()
	return len(pool.ipool)
}

func (pool *SessionPool) Size() int {
	pool.rwmutex.RLock()
	defer pool.rwmutex.RUnlock()
	return len(pool.ipool) + len(pool.bpool)
}

func (pool *SessionPool) BSize() int {
	pool.rwmutex.RLock()
	defer pool.rwmutex.RUnlock()
	return len(pool.bpool)
}

func (pool *SessionPool) PushSession(conid int) bool {
	pool.rwmutex.Lock()
	defer pool.rwmutex.Unlock()
	temp := pool.bpool[conid]
	pool.ipool[conid] = temp
	delete(pool.bpool, conid)
	return true
}

func (pool *SessionPool) PopSession() (*asyncsession.AsyncSession, bool) {
	pool.rwmutex.Lock()
	defer pool.rwmutex.Unlock()
	if len(pool.ipool) == 0 {
		pool.Malloc(1 + pool.count/2)
	}
	var temp *asyncsession.AsyncSession
	for k, v := range pool.ipool {
		temp = v
		pool.bpool[k] = temp
		delete(pool.ipool, k)
	}
	return temp, true
}
