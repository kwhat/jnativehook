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
package org.jnativehook;

import org.junit.Assert.*;

public class PropertiesTest {
	@Before
	public void setUp() {
		try {
			hashtags = new HashtagsTable();
		}
		catch (SQLException ex) {
			fail(ex.getMessage());
		}
	}

	@After
	public void tearDown() {
		hashtags = null;

		// TODO Should delete what we inserted.
	}

	@Test
	public void testInsert() {
		Hashtag tag1 = new Hashtag("Hashtag 1");
		Hashtag tag2 = new Hashtag("Hashtag 2");

		hashtags.add(tag1);
		hashtags.add(tag2);

		int[] status;
		try {
			status = hashtags.executeInserts();
			for (int i = 0; i < status.length; i++) {
				// Check that the number of affected rows is not zero.
				assertNotEquals(status[i], 0);
			}
		}
		catch (SQLException ex) {
			fail(ex.getMessage());
		}

		List<Integer> keys = hashtags.getInsertedKeys();
		assertEquals(keys.size(), 2);

		// Make sure the get method returns the two keys we inserted.
		assertEquals(hashtags.get(keys.get(0)).getText(), tag1.getText());
		assertEquals(hashtags.get(keys.get(1)).getText(), tag2.getText());
	}
}
