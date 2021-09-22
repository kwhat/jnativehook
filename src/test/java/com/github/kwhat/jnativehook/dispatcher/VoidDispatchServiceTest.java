/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2021 Alexander Barker.  All Rights Reserved.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.github.kwhat.jnativehook.dispatcher;

import org.junit.jupiter.api.Test;
import java.util.List;
import java.util.concurrent.TimeUnit;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class VoidDispatchServiceTest {
    @Test
    public void testConstructor() {
        VoidDispatchService service = new VoidDispatchService();

        assertFalse(service.isShutdown());
        assertFalse(service.isTerminated());
    }

    @Test
    public void testShutdown() {
        VoidDispatchService service = new VoidDispatchService();
        service.shutdown();

        assertTrue(service.isShutdown());
        assertTrue(service.isTerminated());
    }

    @Test
    public void testShutdownNow() {
        VoidDispatchService service = new VoidDispatchService();
        List<Runnable> tasks = service.shutdownNow();

        assertEquals(0, tasks.size());
        assertTrue(service.isShutdown());
        assertTrue(service.isTerminated());
    }

    @Test
    public void testAwaitTermination() {
        VoidDispatchService service = new VoidDispatchService();

        assertTrue(service.awaitTermination(1L, TimeUnit.SECONDS));
    }

    @Test
    public void testExecute() {
        VoidDispatchService service = new VoidDispatchService();

        final boolean[] hasRun = {false};
        service.execute(new Runnable() {
            @Override
            public void run() {
                hasRun[0] = true;
            }
        });

        assertTrue(hasRun[0]);
    }
}
