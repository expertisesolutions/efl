#ifndef EOLIAN_MONO_CSHARP_GENERATORS_HH
#define EOLIAN_MONO_CSHARP_GENERATORS_HH

#include "generation_contexts.hh"
#include "csharp_definitions.hh"

namespace eolian_mono::csharp_definitions::utils {

auto split_as_str(CSharp_Modifiers modifiers) -> std::vector<std::string> {
    auto mods = std::vector<std::string>{};

    if (contains(modifiers, CSharp_Modifiers::PUBLIC)) {
        mods.push_back("public");
    } else if (contains(modifiers, CSharp_Modifiers::PRIVATE)) {
        mods.push_back("private");
    } else if (contains(modifiers, CSharp_Modifiers::PROTECTED)) {
        mods.push_back("protected");
    } else if (contains(modifiers, CSharp_Modifiers::INTERNAL)) {
        mods.push_back("internal");
    }

    if (contains(modifiers, CSharp_Modifiers::STATIC)) {
        mods.push_back("static");
    } else if (contains(modifiers, CSharp_Modifiers::CONST)) {
        mods.push_back("const");
    }

    if (contains(modifiers, CSharp_Modifiers::READ_ONLY)) {
        mods.push_back("readonly");
    }

    if (contains(modifiers, CSharp_Modifiers::OUT)) {
        mods.push_back("out");
    } else if (contains(modifiers, CSharp_Modifiers::REF)) {
        mods.push_back("ref");
    }

    return mods;
}

}

namespace eolian_mono::csharp_definitions {
/**
 *
 * Straight forward generators - to be moved (TODO).
 * -------------------------------------------------
 *
 */

using namespace efl::eolian::grammar;

// Function calls
struct Call_Terminal
{
    struct Generator {
        Generator() = default;
        Generator(std::vector<std::string> args):
            args{args}
        {}

        template <typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, CSharp_Function const& function, Context const& context) const
        {
            return as_generator(
                        function.decl.name << "(" << (string % ", ") << ")"
                    ).generate(sink, args, context);
        }

        std::vector<std::string> args;
    };

    auto operator()() const -> Generator {
        return {};
    }

    auto operator()(std::vector<std::string> const& args) const -> Generator {
        return {args};
    }

    auto operator()(std::vector<Decl> const& args) const -> Generator {
        auto v = std::vector<std::string>{};
        for (auto const& arg: args) {
            auto splitted_modifiers = utils::split_as_str(arg.modifiers);

            auto arg_str = std::string{};
            for (auto const& mod: splitted_modifiers) {
                arg_str += mod + " ";
            }
            arg_str += arg.name;

            v.push_back(arg_str);
        }
        return {v};
    }
} const Call;

auto as_generator(Call_Terminal) -> Call_Terminal::Generator {
    return {{}};
}

}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Call_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Call_Terminal> : std::true_type {};
template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Call_Terminal::Generator> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Call_Terminal::Generator> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Call_Terminal> : std::integral_constant<int, 1> {};
template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Call_Terminal::Generator> : std::integral_constant<int, 1> {};

}

}

namespace eolian_mono::csharp_definitions {

struct Type_Generator {
    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, Type const& _type, Context const& context) const
    {
        return as_generator(
                 attribute_conditional([](std::vector<SingleType> const& types) { return types.size() > 1; }) << (
                     "(" << (type(true) % ", ") << ")"
                 ) | *type(true) 
               ).generate(sink
                          , std::make_tuple(_type.types, _type.types)
                          , context);
    }
};

}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Type_Generator> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Type_Generator> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Type_Generator> : std::integral_constant<int, 1> {};

}

}

namespace eolian_mono::csharp_definitions {

Type_Generator const Type_Gen = {};

// Every possible C# declaration (but not definitions)
struct Decl_Terminal {
    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, Decl const& decl, Context const& context) const
    {
        auto mods = utils::split_as_str(decl.modifiers);

        if (!as_generator(*(string << " "))
             .generate(sink, mods, context))
            return false;

        if (!as_generator(Type_Gen << " " << decl.name)
             .generate(sink, decl.type, context))
            return false;
        return true;
    }

    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, CSharp_Property::Getter const& getter, Context const& context) const
    {
        auto const& indent = current_indentation(context);

        if (!as_generator(lit("get")).generate(sink, attributes::unused, context))
            return false;

        if (getter.native_call) {
            auto f = *getter.native_call;
            auto param_types = std::vector<attributes::type_def>{};
            auto param_names = std::vector<std::string>{};
            auto param_pair = std::vector<std::tuple<attributes::type_def, std::string>>{};

            auto get_args = std::vector<Decl>{};

            for (auto const& param: f.parameters) {
                param_types.push_back(param.type.types[0]);
                param_names.push_back("_out_" + param.name);
                param_pair.push_back({param_types.back(), param_names.back()});

                auto get_arg = param;
                get_arg.name = "_out_" + get_arg.name;
                get_args.push_back(get_arg);
            }

            if (!as_generator(lit(" {")).generate(sink, attributes::unused, context))
                return false;

            if (f.parameters.size() >= 1) {
                auto block_context = change_indentation(indent.inc(), context);
                auto const& indent = current_indentation(block_context);
                if (!as_generator(
                        "\n" <<
                        *attribute_reorder<0, 1, 0>(
                            indent << scope_tab << type(true) << " " << string << " = default(" << type(true) << ");\n"
                        )
                    ).generate(sink, param_pair, block_context))
                    return false;
                if (f.decl.type.types[0].c_type == "Eina_Success_Flag") {
                    auto error_msg = "\"Call of native function for " + f.decl.name + " returned an error.\"";
                    if (!as_generator(""
                            << indent << scope_tab << "var success = " << Call(get_args) << ";\n"
                            << indent << scope_tab << "if (!success) {\n"
                            << indent << scope_tab << scope_tab << "throw new Efl.EflException(" << error_msg.c_str() << ");\n"
                            << indent << scope_tab << "}\n"
                        ).generate(sink, f, block_context))
                    return false;
                } else {
                    if (!as_generator(
                            indent << scope_tab << Call(get_args) << ";\n"
                        ).generate(sink, f, block_context))
                        return false;
                }

                if (!as_generator(
                        indent << scope_tab << "return (" << (attribute_reorder<1>(string) % ", ") << ");\n"
                    ).generate(sink, param_pair, block_context))
                    return false;

                if (!as_generator(indent << "}").generate(sink, attributes::unused, block_context))
                    return false;
            } else {
                if (!as_generator(
                         " return " << Call() << "; }"
                    ).generate(sink, f, context))
                    return false;
            }
        } else {
            if (!as_generator(lit(";"))
                 .generate(sink, attributes::unused, context))
                return false;
        }

        return true;
    }

    template<typename OutputIterator, typename Context>
    bool generate(OutputIterator sink, CSharp_Property::Setter const& setter, Context const& context) const
    {
        auto const& indent = current_indentation(context);
        auto set_args = std::vector<Decl>{};
        if (setter.native_call) {
            auto f = *setter.native_call;

            if (f.parameters.size() > 1) {
                auto i = 1;
                for (auto const& param: f.parameters) {
                    auto set_arg = param;
                    set_arg.name = "value.Item" + std::to_string(i);
                    set_args.push_back(set_arg);
                    ++i;
                }
            } else {
                auto set_arg = f.parameters[0];
                set_arg.name = "value";
                set_args.push_back(set_arg);
            }
        }

        auto splitted_modifiers = utils::split_as_str(setter.modifiers);
        if (!as_generator(*(string << " ") << lit("set")).generate(sink, splitted_modifiers, context))
            return false;

        if (setter.native_call) {
            auto f = *setter.native_call;

            if (f.decl.type.types[0].c_type == "Eina_Success_Flag") {
                auto block_context = change_indentation(indent.inc(), context);
                auto const& indent = current_indentation(block_context);

                auto error_msg = "\"Call of native function for " + f.decl.name + " returned an error.\"";
                if (!as_generator(
                        " {\n"
                        << indent << scope_tab << "var success = " << Call(set_args) << ";\n"
                        << indent << scope_tab << "if (!success) {\n"
                        << indent << scope_tab << scope_tab << "throw new Efl.EflException(" << error_msg.c_str() << ");\n"
                        << indent << scope_tab << "}\n"
                        << indent << "}"
                    ).generate(sink, f, block_context))
                return false;
            } else {
                if (!as_generator(" { " << Call(set_args) << "; }")
                     .generate(sink, f, context))
                    return false;
            }
        } else {
            if (!as_generator(lit(";")).generate(sink, attributes::unused, context))
                return false;
        }
        return true;
    }
} const Decl {};

}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Decl_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Decl_Terminal> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Decl_Terminal> : std::integral_constant<int, 1> {};

}

}

// General C# code (TODO: Specify to Block?)
namespace eolian_mono::csharp_definitions {

using efl::eolian::grammar::scope_tab;

struct Definition_Terminal {
    static const Definition_Terminal Definition;

    struct Generator {
        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, csharp_definitions::CSharp_Function const& function, Context const& context) const
        {
            // TODO
            return true;
        }

        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, csharp_definitions::CSharp_Property const& property, Context const& context) const
        {
            auto const& indent = current_indentation(context);
            return as_generator(
                indent << Decl << " {\n" <<
                ((!(indent << scope_tab << Decl << "\n")) | eps) << // getter
                ((!(indent << scope_tab << Decl << "\n")) | eps) << // setter
                indent << "}\n")
                .generate(sink
                          , std::make_tuple(property.decl, property.getter, property.setter)
                          , context);
        }
    };
};

auto as_generator(Definition_Terminal) -> Definition_Terminal::Generator const {
    return {};
}

}

namespace efl::eolian::grammar {

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Definition_Terminal::Generator> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Definition_Terminal::Generator> : std::true_type {};

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Definition_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Definition_Terminal> : std::true_type {};

}

// Assignment operator
namespace eolian_mono::csharp_definitions {

const Definition_Terminal Definition_Terminal::Definition = Definition_Terminal{};
Definition_Terminal const& Definition = Definition_Terminal::Definition;

struct Assign_Terminal {
    struct Generator {
        Generator(csharp_definitions::CSharp_Variable& var, std::string expr):
            var{var},
            expr{std::move(expr)}
        {}

        template<typename OutputIterator, typename Context>
        bool generate(OutputIterator sink, attributes::unused_type, Context const& context) const
        {
            if (!as_generator(efl::eolian::grammar::lit("test") <<
                   (!var.initialized ? "var " : "") << var.name << " = " << expr
                 ).generate(sink, attributes::unused, context))
              return false;

            var.initialized = true;

            return true;
        }

        csharp_definitions::CSharp_Variable& var;
        std::string expr;
    };

    auto operator()(csharp_definitions::CSharp_Variable var, std::string expr) const -> Generator {
        return {var, expr};
    }
} const Assign;

}

namespace efl { namespace eolian { namespace grammar {

template <>
struct is_generator<::eolian_mono::csharp_definitions::Assign_Terminal> : std::true_type {};
template <>
struct is_generator<::eolian_mono::csharp_definitions::Assign_Terminal::Generator> : std::true_type {};

template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Assign_Terminal> : std::true_type {};
template <>
struct is_eager_generator<::eolian_mono::csharp_definitions::Assign_Terminal::Generator> : std::true_type {};

namespace type_traits {

template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Definition_Terminal> : std::integral_constant<int, 1> {};
template <>
struct attributes_needed<::eolian_mono::csharp_definitions::Definition_Terminal::Generator> : std::integral_constant<int, 1> {};

}

}}}

#endif
