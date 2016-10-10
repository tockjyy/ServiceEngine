package asyncsession

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"net"
	"proto/protoqueue"
	"sync"
)

type AsyncSession struct {
	conn   net.Conn
	wbuf   bytes.Buffer
	rbuf   bytes.Buffer
	wmutex sync.Mutex
	rmutex sync.Mutex
	issend bool
	conid  int
	lname  string
}

func (session *AsyncSession) SetConnectIdAndName(id int, name string) {
	session.conid = id
	session.lname = name
}

func (session *AsyncSession) GetConnectId() int {
	return session.conid
}

func (session *AsyncSession) GetName() string {
	return session.lname
}

func (session *AsyncSession) RecvHandle(conn net.Conn) {
	session.conn = conn
	defer session.conn.Close()
	session.rbuf.Reset()
	session.wbuf.Reset()
	session.issend = false
	fmt.Println("suceess connect...")
	for {
		buf := make([]byte, 1024)
		count, err := session.conn.Read(buf)
		if err != nil {
			println("Error reading:", err.Error())
			return
		}
		session.rmutex.Lock()
		session.rbuf.Write(buf[:count])

		b_buf := bytes.NewBuffer(session.rbuf.Bytes()[0:2])
		var x uint16
		binary.Read(b_buf, binary.LittleEndian, &x)
		num := int(x + 2)
		if num > session.rbuf.Len() {
			session.rmutex.Unlock()
			continue
		} else {
			println("reading:", session.rbuf.String(), " len:", count)
			h_buf := make([]byte, num)
			session.rbuf.Read(h_buf[:num])
			protoqueue.Instance().PushRecvQueue(session.lname, session.conid, h_buf)
		}
		session.rmutex.Unlock()
	}
}

func (session *AsyncSession) WriteBuffer(buf []byte) {
	println("lock")
	session.wmutex.Lock()
	session.wbuf.Write(buf)
	session.wmutex.Unlock()
	session.CanSend()
}

func (session *AsyncSession) SendHandle(size int) {
	snedbuf := make([]byte, size)
	session.wbuf.Read(snedbuf)
	count, err := session.conn.Write(snedbuf)
	if err != nil {
		println("Error sending:", err.Error())
		return
	}
	println("writing:", session.wbuf.String(), " len:", count)
	if count != size {
		println("datasize is error!")
	}
	session.issend = false
}

func (session *AsyncSession) CanSend() {
	if session.issend == true {
		return
	}
	println("lock")
	session.wmutex.Lock()
	defer session.wmutex.Unlock()
	b_buf := bytes.NewBuffer(session.wbuf.Bytes()[0:2])
	var x uint16
	binary.Read(b_buf, binary.LittleEndian, &x)
	fmt.Println(x)
	num := int(x + 2)
	if num < session.wbuf.Len() {
		return
	} else {
		session.issend = true
		session.SendHandle(num)
	}
}
