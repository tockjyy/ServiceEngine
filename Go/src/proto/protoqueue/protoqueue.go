package protoqueue

var _instance *ProtoQueue

func Instance() *ProtoQueue {
	if _instance == nil {
		_instance = new(ProtoQueue)
		_instance.recv_task_queue = make(chan Task)
		_instance.send_task_queue = make(chan Task)
	}
	return _instance
}

type Task struct {
	name  string
	conid int
	buf   []byte
}

func (t *Task) Name() string {
	return t.name
}

func (t *Task) Conid() int {
	return t.conid
}

func (t *Task) Buf() []byte {
	return t.buf
}

type ProtoQueue struct {
	recv_task_queue chan Task
	send_task_queue chan Task
}

func NewTask(name string, conid int, buf []byte) Task {
	task := Task{name, conid, buf}
	task.name = name
	task.conid = conid
	task.buf = buf
	return task
}

func (queue *ProtoQueue) PushRecvQueue(name string, conid int, buf []byte) {
	task := Task{name, conid, buf}
	task.name = name
	task.conid = conid
	task.buf = buf
	queue.recv_task_queue <- task
}

func (queue *ProtoQueue) PushSendQueue(task Task) {
	queue.send_task_queue <- task
}

func (queue *ProtoQueue) GetRecvQueue() chan Task {
	return queue.recv_task_queue
}

func (queue *ProtoQueue) GetSendQueue() chan Task {
	return queue.send_task_queue
}
