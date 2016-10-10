package protohandle

import "proto/protoqueue"

func HandleProto(task protoqueue.Task) {
	go func(task protoqueue.Task) {
		task_ := protoqueue.NewTask(task.Name(), task.Conid(), task.Buf())
		protoqueue.Instance().PushSendQueue(task_)
	}(task)
}
