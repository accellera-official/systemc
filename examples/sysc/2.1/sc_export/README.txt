The sc_export class is an addition to ports and allow one to export an
interface through the module hierarchy. The sc_export makes an
interface - that is bound to a channel located somewhere within that
module - available to the outside of the module. If you see a module with an 
sc_export instance then you can be sure that this module already has a channel 
bound to that sc_export.


Binding
---

sc_export instances are similar to ports with respect to binding. An
sc_export can be bound to either a channel or another sc_export,
given that this sc_export itself is directly or indirectly
bound to a channel. Types must match which is checked either 
during compilation or elaboration

Binding can be done by name, or by CTOR. Binding is
generally done just like ports, except for the following:

If you bind an sc_export to another sc_export like port
E.IFP2 to D.IFP in the example below, then you must bind the
parent_expp to the child_exp, e.g. "parent_exp(child_exp)". For ports,
this is done the opposite way like child_port(parent_port),
however this is wrong for sc_exports and leads to an error during
elaboration.

As a rule of thumb, bind "further(closer)" with "further" the
port/export that is further away from the channel. This
further(closer) rule works for ports as well as exports. For
hierarchical ports, the channel is connected to the port of the
top-most module, so parent_port==closer, hence do a
child_port(parent_port). For an sc_export, the channel is
embedded to the innermost instance, so child_exp==closer, hence do
parent_exp(child_exp)

Names
---

An sc_export can be given an explicit name through the CTOR. If
not, then a default name like "export_0", "export_1", ... is given
with an individual number set for each module.


Supported Functions and Restrictions
---

You can access the interface of an export with the get_interface()
method as well as with operator ->.

It is not allowed to use an export as an argument in the sensitivity
list of a process. Furthermore, exports are not allowed in lambda
expressions. Processes can use ports but not exports in these
contexts.
