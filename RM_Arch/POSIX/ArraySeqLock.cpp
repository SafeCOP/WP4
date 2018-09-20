#include "ArraySeqLock.h"

ArraySeqLock::ArraySeqLock(const length_t array_length) : sequence(), tmp_sequence(), array_length(array_length) { }

length_t ArraySeqLock::write_get_index() {
    if (++tmp_sequence.writer_index >= array_length) {
        tmp_sequence.writer_index = 0;
        ++tmp_sequence.wrap_count;
    }
    tmp_sequence.writing = true;

    return tmp_sequence.writer_index;
}

void ArraySeqLock::write_seqbegin() {
    sequence.store(tmp_sequence, memory_order_seq_cst);
}

void ArraySeqLock::write_seqend() {
    tmp_sequence.writing = false;
    sequence.store(tmp_sequence, memory_order_seq_cst);
}

struct array_seq_lock ArraySeqLock::read_seqbegin(const length_t index) const {
    struct array_seq_lock tmp;
    do tmp = sequence.load(memory_order_relaxed);
    while (tmp.writer_index == index && tmp.writing);

    atomic_thread_fence(memory_order_seq_cst);
    return tmp;
}

bool ArraySeqLock::read_seqretry(const struct array_seq_lock &seq_lock, const length_t index) const {
    struct array_seq_lock tmp = sequence.load(memory_order_seq_cst);

    if (seq_lock.wrap_count == tmp.wrap_count) {
        if (!(seq_lock.writer_index < index && tmp.writer_index >= index))
            return false;
    } else if (seq_lock.wrap_count + 1 == tmp.wrap_count) {
        if (seq_lock.writer_index >= index && tmp.writer_index < index)
            return false;
    }
    return true;
}

length_t ArraySeqLock::getLength() const {
    return array_length;
}
