package org.jnativehook.keyboard;

import java.awt.event.KeyEvent;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

public class NativeKeySet implements List<KeyEvent> {
	private final int step = 1;
	private int size;
	private KeyEvent[] backing;
	
	public NativeKeySet() {
		clear();
	}
	
	@Override
	public void add(int index, KeyEvent element) {
		// TODO Auto-generated method stub
	}

	public boolean add(KeyEvent e) {
		boolean bReturn = false;
		
		if (!contains(e)) {
			if (size >= backing.length) {
				backing = Arrays.copyOf(backing, size + step);
			}
			
			backing[size++] = e;
			bReturn = true;
		}
		
		return bReturn;
	}

	@Override
	public boolean addAll(int index, Collection<? extends KeyEvent> c) {
		// TODO Auto-generated method stub
		return false;
	}
	
	public boolean addAll(Collection<? extends KeyEvent> c) {
		boolean bReturn = false;
		Iterator<? extends KeyEvent> i = c.iterator();
		
		while (i.hasNext()) {
			bReturn = bReturn || add(i.next());
		}
		
		return bReturn;
	}

	public void clear() {
		size = 0;
		backing = new KeyEvent[step];
	}

	public boolean contains(Object o) {
		return indexOf(o) > -1;
	}

	public boolean containsAll(Collection<?> c) {
		boolean bReturn = true;
		Iterator<?> i = c.iterator();
		
		while (i.hasNext() && bReturn) {
			bReturn = bReturn && contains(i.next());
		}
		
		return bReturn;
	}

	@Override
	public KeyEvent get(int index) {
		return backing[index];
	}

	public int indexOf(Object o) {
		int iReturn = -1;
		
		if (o instanceof KeyEvent) {
			for (int i = 0; i < size && iReturn == -1; i++) {
				if (backing[i].getKeyCode() == ((KeyEvent) o).getKeyCode() && 
					backing[i].getModifiers() == ((KeyEvent) o).getModifiers() &&
					backing[i].getKeyLocation() == ((KeyEvent) o).getKeyLocation()
				) {
					iReturn = i;
				}
			}
		}
		
		return iReturn;
	}

	public boolean isEmpty() {
		return size < 1;
	}

	public Iterator<KeyEvent> iterator() {
		return Arrays.asList(Arrays.copyOf(backing, size)).iterator();
	}

	@Override
	public int lastIndexOf(Object o) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public ListIterator<KeyEvent> listIterator() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ListIterator<KeyEvent> listIterator(int index) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public KeyEvent remove(int index) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public boolean remove(Object o) {
		boolean bReturn = false;
		
		int i = indexOf(o);
		if (i > -1) {
			KeyEvent[] tmp = new KeyEvent[size - 1];
			System.arraycopy(backing, 0, tmp, 0, i);
			System.arraycopy(backing, i + 1, tmp, i + 1, size);
			
			bReturn = true;
		}
		
		return bReturn;
	}

	public boolean removeAll(Collection<?> c) {
		boolean bReturn = false;
		Iterator<?> i = c.iterator();
		
		while (i.hasNext()) {
			bReturn = bReturn || remove(i.next());
		}
		
		return bReturn;
	}

	@Override
	public boolean retainAll(Collection<?> c) {
		boolean bReturn = false;
		
		Iterator<?> i = c.iterator();
		while (i.hasNext()) {
			Object objItem = i.next();
			
			if (indexOf(objItem) < 0) { 
				c.remove(objItem);
			}
		}
		
		bReturn = c.size() != size;
		size = c.size();
		backing = c.toArray(backing);
		
		return bReturn;
	}

	@Override
	public KeyEvent set(int index, KeyEvent element) {
		// TODO Auto-generated method stub
		return null;
	}
	
	public int size() {
		return size;
	}
	
	@Override
	public List<KeyEvent> subList(int fromIndex, int toIndex) {
		// TODO Auto-generated method stub
		return null;
	}

	public Object[] toArray() {
		return Arrays.copyOf(backing, size);
	}

	public <T> T[] toArray(T[] a) {
		if (a.length < size) {
			a = Arrays.copyOf(a, size);
		}
		
		System.arraycopy(backing, 0, a, 0, size);
		
		return a;
	}
}
