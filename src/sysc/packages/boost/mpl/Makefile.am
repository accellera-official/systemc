## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2004 by all Contributors.
##  All Rights reserved.
##
##  The contents of this file are subject to the restrictions and limitations
##  set forth in the SystemC Open Source License Version 2.4 (the "License");
##  You may not use this file except in compliance with such restrictions and
##  limitations. You may obtain instructions on how to receive a copy of the
##  License at http://www.systemc.org/. Software distributed by Contributors
##  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
##  ANY KIND, either express or implied. See the License for the specific
##  language governing rights and limitations under the License.
##
## ****************************************************************************
##
##  src/sysc/packages/boost/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Andy Goodrich, Forte Design Systems, Inc., 2003-05-21
##
## ****************************************************************************
##
##  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
##  changes you are making here.
##
##      Name, Affiliation, Date:  
##  Description of Modification:
##      Andy Goodrich, Forte Design Systems
##         New directory for boost package.
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules

H_FILES = \
	bool.hpp \
	bool_fwd.hpp \
	integral_c_tag.hpp 

MAYBE = \
	O1_size.hpp \
	O1_size_fwd.hpp \
	accumulate.hpp \
	advance.hpp \
	advance_fwd.hpp \
	alias.hpp \
	always.hpp \
	and.hpp \
	apply.hpp \
	apply_fwd.hpp \
	apply_wrap.hpp \
	arg.hpp \
	arg_fwd.hpp \
	arithmetic.hpp \
	as_sequence.hpp \
	assert.hpp \
	at.hpp \
	at_fwd.hpp \
	back.hpp \
	back_fwd.hpp \
	back_inserter.hpp \
	base.hpp \
	begin.hpp \
	begin_end.hpp \
	begin_end_fwd.hpp \
	bind.hpp \
	bind_fwd.hpp \
	bitand.hpp \
	bitor.hpp \
	bitwise.hpp \
	bitxor.hpp \
	clear.hpp \
	clear_fwd.hpp \
	comparison.hpp \
	contains.hpp \
	contains_fwd.hpp \
	copy.hpp \
	copy_if.hpp \
	count.hpp \
	count_fwd.hpp \
	count_if.hpp \
	deque.hpp \
	deref.hpp \
	distance.hpp \
	distance_fwd.hpp \
	divides.hpp \
	empty.hpp \
	empty_base.hpp \
	empty_fwd.hpp \
	empty_sequence.hpp \
	end.hpp \
	equal.hpp \
	equal_to.hpp \
	erase.hpp \
	erase_fwd.hpp \
	erase_key.hpp \
	erase_key_fwd.hpp \
	eval_if.hpp \
	filter_view.hpp \
	find.hpp \
	find_if.hpp \
	fold.hpp \
	for_each.hpp \
	front.hpp \
	front_fwd.hpp \
	front_inserter.hpp \
	greater.hpp \
	greater_equal.hpp \
	has_key.hpp \
	has_key_fwd.hpp \
	has_xxx.hpp \
	identity.hpp \
	if.hpp \
	index_if.hpp \
	index_of.hpp \
	inherit.hpp \
	inherit_linearly.hpp \
	insert.hpp \
	insert_fwd.hpp \
	insert_range.hpp \
	insert_range_fwd.hpp \
	inserter.hpp \
	int.hpp \
	int_fwd.hpp \
	integral_c.hpp \
	integral_c_fwd.hpp \
	is_placeholder.hpp \
	is_sequence.hpp \
	iter_fold.hpp \
	iter_fold_if.hpp \
	iterator_category.hpp \
	iterator_range.hpp \
	iterator_tags.hpp \
	joint_view.hpp \
	key_type.hpp \
	key_type_fwd.hpp \
	lambda.hpp \
	lambda_fwd.hpp \
	less.hpp \
	less_equal.hpp \
	list.hpp \
	list_c.hpp \
	logical.hpp \
	long.hpp \
	long_fwd.hpp \
	lower_bound.hpp \
	map.hpp \
	max.hpp \
	max_element.hpp \
	min.hpp \
	min_element.hpp \
	min_max.hpp \
	minus.hpp \
	modulus.hpp \
	multiplies.hpp \
	negate.hpp \
	next.hpp \
	next_prior.hpp \
	not.hpp \
	not_equal_to.hpp \
	numeric_cast.hpp \
	or.hpp \
	order.hpp \
	order_fwd.hpp \
	pair.hpp \
	pair_view.hpp \
	partition.hpp \
	placeholders.hpp \
	plus.hpp \
	pop_back.hpp \
	pop_back_fwd.hpp \
	pop_front.hpp \
	pop_front_fwd.hpp \
	print.hpp \
	prior.hpp \
	protect.hpp \
	push_back.hpp \
	push_back_fwd.hpp \
	push_front.hpp \
	push_front_fwd.hpp \
	quote.hpp \
	range_c.hpp \
	remove.hpp \
	remove_if.hpp \
	replace.hpp \
	replace_if.hpp \
	reverse.hpp \
	reverse_fold.hpp \
	reverse_iter_fold.hpp \
	same_as.hpp \
	sequence_tag.hpp \
	sequence_tag_fwd.hpp \
	set.hpp \
	set_c.hpp \
	shift_left.hpp \
	shift_right.hpp \
	single_view.hpp \
	size.hpp \
	size_fwd.hpp \
	size_t.hpp \
	size_t_fwd.hpp \
	sizeof.hpp \
	sort.hpp \
	stable_partition.hpp \
	switch.hpp \
	tag.hpp \
	times.hpp \
	transform.hpp \
	transform_view.hpp \
	unique.hpp \
	unpack_args.hpp \
	upper_bound.hpp \
	value_type.hpp \
	value_type_fwd.hpp \
	vector.hpp \
	vector_c.hpp \
	void.hpp \
	void_fwd.hpp \
	zip_view.hpp 

noinst_HEADERS = $(H_FILES)

CXX_FILES = 

SUBDIRS = \
	aux_


INCLUDE_DIR = $(prefix)/include/sysc/packages/boost/mpl

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/$$file; \
	done
	-rmdir $(INCLUDE_DIR)

## Taf!
