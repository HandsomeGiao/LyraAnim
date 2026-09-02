import js from "@eslint/js";
import tseslint from "typescript-eslint";

export default [
    {
        ignores: [
            "node_modules/**",
            "Binaries/**",
            "Intermediate/**",
            "Saved/**",
            "DerivedDataCache/**",
            "JavaScript/**",
            "dist/**",
            "**/*.js",
            "**/*.d.ts",
        ],
    },

    js.configs.recommended,

    ...tseslint.configs.recommended,

    {
        files: ["TypeScript/**/*.ts"],
        rules: {
            "@typescript-eslint/no-explicit-any": "off",

            "@typescript-eslint/no-unused-vars": "off",
            "@typescript-eslint/no-empty-function": "off",
            "@typescript-eslint/no-empty-object-type": "off",
            "@typescript-eslint/no-unsafe-declaration-merging": "off",
        },
    },
];
