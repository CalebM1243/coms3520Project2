main:
	gcc filesystem.c journal.c block_service.c -lpthread -o journal

buffertest:
	gcc test_buffer.c buffer.c -o test_buffer -lpthread

clean:
	rm -f journal test_buffer