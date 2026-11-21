/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2026 Alexander Barker.  All Rights Reserved.
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
import com.github.kwhat.jnativehook.dispatcher.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;
import javax.swing.SwingUtilities;

public class DispatcherTest {
    public static void main(String[] args) throws Exception {
        VoidDispatchService direct = new VoidDispatchService();
        check(!direct.awaitTermination(0, TimeUnit.SECONDS));
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        Thread worker = new Thread(() -> direct.execute(() -> {
            entered.countDown();
            await(release);
        }));
        worker.start();
        check(entered.await(5, TimeUnit.SECONDS));
        direct.shutdown();
        check(!direct.isTerminated());
        reject(direct);
        release.countDown();
        check(direct.awaitTermination(5, TimeUnit.SECONDS));
        worker.join();

        SwingDispatchService swing = new SwingDispatchService();
        CountDownLatch edtBlocked = new CountDownLatch(1);
        CountDownLatch unblockEdt = new CountDownLatch(1);
        SwingUtilities.invokeLater(() -> {
            edtBlocked.countDown();
            await(unblockEdt);
        });
        check(edtBlocked.await(5, TimeUnit.SECONDS));
        AtomicBoolean ran = new AtomicBoolean();
        Runnable task = () -> ran.set(true);
        swing.execute(task);
        swing.shutdown();
        check(!swing.isTerminated());
        reject(swing);
        check(swing.shutdownNow().get(0) == task);
        check(swing.isTerminated());
        unblockEdt.countDown();
        SwingUtilities.invokeAndWait(() -> { });
        check(!ran.get());
        System.out.println("Dispatcher regressions passed");
    }
    private static void reject(ExecutorService service) {
        try {
            service.execute(() -> { });
            throw new AssertionError("Accepted a task after shutdown");
        } catch (RejectedExecutionException expected) { }
    }
    private static void await(CountDownLatch latch) {
        try {
            if (!latch.await(5, TimeUnit.SECONDS)) {
                throw new AssertionError("Timed out");
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new AssertionError(e);
        }
    }
    private static void check(boolean condition) {
        if (!condition) {
            throw new AssertionError();
        }
    }
}
