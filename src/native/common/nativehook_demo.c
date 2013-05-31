/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nativehook.h"

// insert event queue here.

int callback_proc(VirtualEvent * const event) {
	// Put an event in the queue.
	pthread_mutex_lock(&m);
	condition = true;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&m);
}

void *dispatch_proc(void *parm) {

}

int main(int argc, const char * argv[]) {
	// TODO
	// Setup event callback.

	int          status;
	int          threadparm = 1;
	pthread_t    threadid;
	int          thread_stat;

	status = pthread_create(&threadid, NULL, threadfunc, (void *) &threadparm);
	if (status <  0) {
		//perror("pthread_create failed");
		exit(1);
	}

	status = pthread_join(threadid, (void *)&thread_stat);
	if (status <  0) {
		perror("pthread_join failed");
		exit(1);
	}
}
