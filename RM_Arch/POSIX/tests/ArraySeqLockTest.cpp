#include "ArraySeqLockTest.h"
#include "../ArraySeqLock.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ArraySeqLockTest);

/**
 * The writer is writing at an index smaller than the readers before and after it finishes reading.
 *
 * [ W |   |   | R |   ]
 *
 * [   | W |   | R |   ]
 *
 * The writer does not interfere with the reader and thus the read should be successful.
 */
void ArraySeqLockTest::writer_before_before() {
    ArraySeqLock lock(5);

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 2; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(!test);
}

/**
 * The writer is writing at an index smaller than the reader before it starts reading, and is at the same index
 * when it finishes writing.
 *
 * [ W |   |   | R |   ]
 *
 * [   |   |   | WR|   ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_before_equal() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 2; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    lock.write_get_index();
    lock.write_seqbegin();
    lock.write_seqend();

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer is writing at an index smaller than the reader before it starts reading, and is at a larger index
 * when it finishes writing.
 *
 * [ W |   |   | R |   ]
 *
 * [   |   |   | W | W ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_before_after() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 2; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 2; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer and the reader are at the same index before the read, the writer has a larger index when
 * the reader has finished reading
 *
 * [   |   | WR|   |   ]
 *
 * [   |   |  R|   | W ]
 *
 * The writer does not interferes with the reader because we wait for it to finish before reading, thus the read should
 * succeed.
 */
void ArraySeqLockTest::writer_equal_after() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 2; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(2);

    for (size_t i = 0; i < 2; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }


    bool test = lock.read_seqretry(lock1, 2);

    CPPUNIT_ASSERT(!test);
}

/**
 * The writer is writing at an index that is larger than the readers before and after it finishes reading.
 *
 * [   |   | R | W |   ]
 *
 * [   |   | R |   | W ]
 *
 * The writer does not interfere with the reader and thus the read should be successful.
 */
void ArraySeqLockTest::writer_after_after() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 3; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(2);

    lock.write_get_index();
    lock.write_seqbegin();
    lock.write_seqend();


    bool test = lock.read_seqretry(lock1, 2);

    CPPUNIT_ASSERT(!test);
}

/**
 * The writer is writing at an index smaller than the readers before it starts reading. The writer then writes the whole
 * buffer and ends up at an index smaller than the readers.
 *
 * [ W |   |   | R |   ]
 *
 * [   |   |   | R | W ]
 *
 * [ W |   |   | R |   ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_wrap_before_before() {
    ArraySeqLock lock(5);

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 5; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer is writing at an index smaller than the readers before it starts reading. The writer then writes the whole
 * buffer and ends up at the same index than the readers.
 *
 * [ W |   |   | R |   ]
 *
 * [   |   |   | R | W ]
 *
 * [   |   |   | WR|   ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_wrap_before_equal() {
    ArraySeqLock lock(5);

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 8; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer is writing at an index smaller than the readers before it starts reading. The writer then writes the whole
 * buffer and ends up at a larger index than the readers.
 *
 * [ W |   |   | R |   ]
 *
 * [   |   |   | R | W ]
 *
 * [   |   |   | R | W  ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_wrap_before_after() {
    ArraySeqLock lock(5);

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 9; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer is writes on the same index than the readers before it starts reading. The writer then writes the whole
 * buffer and ends up at a larger index than the readers.
 *
 * [   |   |   | WR|   ]
 *
 * [   | w |   | R |   ]
 *
 * [   |   |   | R | W ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_wrap_equal_after() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 3; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 6; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer is writes on a larger index than the readers. The writer then writes the whole
 * buffer and ends up at a larger index than the readers.
 *
 * [   |   |   | R | W ]
 *
 * [   | W |   | R |   ]
 *
 * [   |   |   | R | W ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_wrap_after_after() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 4; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 5; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer is writes on a larger index than the readers. The writer then wraps around the buffer and ends up at an
 * index smaller than the readers.
 *
 * [   |   |   | R | W ]
 *
 * [   | W |   | R |   ]
 *
 * The writer doesn't interferes with the reader and thus the read should succeed.
 */
void ArraySeqLockTest::writer_wrap_after_before() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 4; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 2; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(!test);
}

/**
 * The writer is writes on a larger index than the readers. The writer then wraps around the buffer and ends up at the
 * same index than the reader.
 *
 * [   |   |   | R | W ]
 *
 * [   |   |   | WR |   ]
 *
 * The writer interferes with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_wrap_after_equal() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 4; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 4; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer is writes on the same index than the reader. The writer then wraps around the buffer and ends up before
 * the reader.
 *
 * [   |   |   | WR|   ]
 *
 * [   |   | W | R |   ]
 *
 * The writer doesn't interferes with the reader and thus the read should succeed.
 */
void ArraySeqLockTest::writer_wraps_equal_before() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 3; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 4; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(!test);
}

/**
 * The writer is writes on the same index than the reader. The writer then wraps around the buffer and ends up at the
 * same index than the reader.
 *
 * [   |   |   | WR|   ]
 *
 * [   |   |   | WR|   ]
 *
 * The writer does interfere with the reader and thus the read should fail.
 */
void ArraySeqLockTest::writer_wraps_equal_equal() {
    ArraySeqLock lock(5);

    for (size_t i = 0; i < 3; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    struct array_seq_lock lock1 = lock.read_seqbegin(3);

    for (size_t i = 0; i < 5; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }

    bool test = lock.read_seqretry(lock1, 3);

    CPPUNIT_ASSERT(test);
}

/**
 * The writer wraps around the buffer many times.
 */
void ArraySeqLockTest::writer_wraps_many() {
    ArraySeqLock lock(5);

    struct array_seq_lock lock1 = lock.read_seqbegin(3);
    for (size_t i = 0; i < 13; i++) {
        lock.write_get_index();
        lock.write_seqbegin();
        lock.write_seqend();
    }
    bool test = lock.read_seqretry(lock1, 3);
    CPPUNIT_ASSERT(test);
}


